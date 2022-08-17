#include "vre_commandbuffer.hpp"
#include "vre_framebuffer.hpp"
#include "vre_commandpool.hpp"
#include "vre_renderpass.hpp"
#include "vre_renderlist.hpp"
#include "structs/drawable_list.hpp"
#include "../../scene_manager/scene_manager.hpp"
#include "../../shader_manager/uniform_buffer.hpp"
#include "../../shader_manager/shader_combination.hpp"

namespace vre::Renderer::VreRenderer
{
    void VreCommandBuffer::Create()
    {
        setupRenderPassInfo();

        auto framebuffers = *m_vreRenderList->GetVreFrameBuffer()->GetFrameBuffers();

        // resize command buffer containers
        m_commandBuffersOpaqueObjects.resize(framebuffers.size());
        m_commandBuffersTransparentObjects.resize(framebuffers.size());
        m_rerecordingRequested.resize(framebuffers.size(), false);

        allocateCommandBuffers(m_commandBuffersOpaqueObjects, VK_COMMAND_BUFFER_LEVEL_SECONDARY);
        allocateCommandBuffers(m_commandBuffersTransparentObjects, VK_COMMAND_BUFFER_LEVEL_SECONDARY);

        const VkRenderPass renderPass = m_vreRenderList->GetVreRenderPass()->GetRenderPass();

        // record all opaque objects
        for (size_t i = 0; i < m_commandBuffersOpaqueObjects.size(); i++)
        {
            recordCommandBuffer(m_commandBuffersOpaqueObjects, m_vreRenderList->GetOpaqueDrawables(), i, renderPass, framebuffers[i]);
        }

        // record all transparent objects
        for (size_t i = 0; i < m_commandBuffersTransparentObjects.size(); i++)
        {
            recordCommandBuffer(m_commandBuffersTransparentObjects, m_vreRenderList->GetTransparentDrawables(), i, renderPass, framebuffers[i]);
        }

        // only main scene needs the primary command buffers
        if (!m_vreRenderList->IsMainScene())
        {
            return;
        }

        m_commandBuffersPrimary.resize(framebuffers.size());
        allocateCommandBuffers(m_commandBuffersPrimary, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
        const VkExtent2D* pExtent = &VulkanManager::GetInstance()->GetSwapchainInfo()->extent2D;
        for (size_t i = 0; i < m_commandBuffersPrimary.size(); i++)
        {
            recordPrimaryCommandBuffer(i, renderPass, framebuffers[i], pExtent);
        }
    }

    void VreCommandBuffer::Destroy()
    {
        VkDevice device = VulkanManager::GetInstance()->GetDevice();
        VkCommandPool commandPool = VreRenderer::GetInstance()->GetVreCommandPool()->GetCommandPool();
        VulkanInterface::vkFreeCommandBuffers(device, commandPool, static_cast<uint32_t>(m_commandBuffersPrimary.size()), m_commandBuffersPrimary.data());
        VulkanInterface::vkFreeCommandBuffers(device, commandPool, static_cast<uint32_t>(m_commandBuffersOpaqueObjects.size()), m_commandBuffersOpaqueObjects.data());
        VulkanInterface::vkFreeCommandBuffers(device, commandPool, static_cast<uint32_t>(m_commandBuffersTransparentObjects.size()), m_commandBuffersTransparentObjects.data());
        for (size_t i = 0; i < m_commandBuffersPrimary.size(); i++)
        {
            m_commandBuffersPrimary[i] = nullptr;
            m_commandBuffersOpaqueObjects[i] = nullptr;
            m_commandBuffersTransparentObjects[i] = nullptr;
        }
    }

    void VreCommandBuffer::RecordCommandBuffer(uint32_t imageIndex, bool recordTransparent, bool recordOpaque)
    {
        // rerecording command buffers. a command buffer is resetted implicly by rerecording.
        // event driven -> move camera, check if oject order changes -> if yes, rerecord primary
        // changed scene -> update secondary.
        const VkRenderPass renderPass = m_vreRenderList->GetVreRenderPass()->GetRenderPass();
        const VkExtent2D* pExtent = &VulkanManager::GetInstance()->GetSwapchainInfo()->extent2D;

        if (recordTransparent)
        {
            recordCommandBuffer(m_commandBuffersTransparentObjects, m_vreRenderList->GetTransparentDrawables(), imageIndex, renderPass, m_framebuffers->at(imageIndex));
        }

        if (recordOpaque)
        {
            recordCommandBuffer(m_commandBuffersOpaqueObjects, m_vreRenderList->GetOpaqueDrawables(), imageIndex, renderPass, m_framebuffers->at(imageIndex));
        }

        if (m_vreRenderList->IsMainScene())
        {
            if (recordTransparent || recordOpaque)
            {
                recordPrimaryCommandBuffer(imageIndex, renderPass, m_framebuffers->at(imageIndex), pExtent);
            }
        }
    }

    const std::vector<VkCommandBuffer>* VreCommandBuffer::GetCommandBuffers()
    {
        return &m_commandBuffersPrimary;
    }

    void VreCommandBuffer::RecordRenderPass(VkCommandBuffer primaryCommandBuffer, uint32_t imageIndex)
    {
        m_renderPassBeginInfo.framebuffer = m_framebuffers->at(imageIndex);

        VulkanInterface::vkCmdBeginRenderPass(primaryCommandBuffer, &m_renderPassBeginInfo, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);
            // draw opaque objects first
            vkCmdExecuteCommands(primaryCommandBuffer, 1, &m_commandBuffersOpaqueObjects[imageIndex]);

            // draw transparent objects after
            vkCmdExecuteCommands(primaryCommandBuffer, 1, &m_commandBuffersTransparentObjects[imageIndex]);
        VulkanInterface::vkCmdEndRenderPass(primaryCommandBuffer);
    }

    void VreCommandBuffer::RequestRerecording(bool opaqueDrawables, bool transparentDrawables)
    {
        for (size_t i = 0; i < m_rerecordingRequested.size(); i++)
        {
            m_rerecordingRequested[i] = true;
        }

        // notify drawer that at least one render list needs a command buffer rerecording.
        if (opaqueDrawables)
        {
            vre::Renderer::VreRenderer::VreRenderer::GetInstance()->OpaqueDrawablesChanged();
        }
        if (transparentDrawables)
        {
            vre::Renderer::VreRenderer::VreRenderer::GetInstance()->TransparentDrawablesChanged();
        }
    }

    void VreCommandBuffer::recordPrimaryCommandBuffer(uint32_t imageIndex, VkRenderPass renderPass, VkFramebuffer framebuffer, const VkExtent2D* pExtent)
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        checkVk(
            VulkanInterface::vkBeginCommandBuffer(m_commandBuffersPrimary[imageIndex], &beginInfo),
            "VreCommandBuffer::recordPrimaryCommandBuffer: failed to begin recording command buffer!"
        );

        // get all renderlists and record their render pass
        auto renderLists = *VreRenderer::GetInstance()->GetRenderLists();
        for (auto& renderList : renderLists)
        {
            renderList->GetVreCommandBuffer()->RecordRenderPass(m_commandBuffersPrimary[imageIndex], imageIndex);
        }

        checkVk(
            VulkanInterface::vkEndCommandBuffer(m_commandBuffersPrimary[imageIndex]),
            "VreCommandBuffer::recordPrimaryCommandBuffer: failed to record command buffer!"
        );
    }

    void VreCommandBuffer::recordCommandBuffer(
        const std::vector<VkCommandBuffer>& commandBuffers,
        const std::vector<DrawableList>* drawableLists,
        uint32_t index,
        VkRenderPass renderPass,
        VkFramebuffer framebuffer)
    {
        // secondary command buffers need an inheritance info
        VkCommandBufferInheritanceInfo inheritInfo{};
        inheritInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
        inheritInfo.renderPass = renderPass;
        inheritInfo.framebuffer = framebuffer;

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.pInheritanceInfo = &inheritInfo;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;

        checkVk(
            VulkanInterface::vkBeginCommandBuffer(commandBuffers[index], &beginInfo),
            "VreCommandBuffer::recordCommandBuffer: failed to begin recording command buffer!"
        );

        for (const auto& drawableList : *drawableLists)
        {
            std::shared_ptr<VrePipelineSet> pipelineSet = drawableList.mPipelineSet.lock();
            if (pipelineSet == nullptr)
            {
                auto scIndex = drawableList.mShaderCombination->GetShaderCombinationIndex();
                auto pipelineSets = m_vreRenderList->GetPipelineSets();
                auto it = pipelineSets->find(scIndex);
                if(it == pipelineSets->end())
                {
                    err("VreCommandBuffer::recordCommandBuffer: failed to get pipeline set for this drawable list!");
                    return;
                }
                pipelineSet = it->second;
                
            }
            VulkanInterface::vkCmdBindPipeline(commandBuffers[index], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineSet->mPipeline);

            for (const auto& drawableData : drawableList.mDrawableDataList)
            {
                VkDescriptorSet descriptorSet = drawableData->mUniformBuffer->GetDescriptorSet(index);
                vkCmdBindDescriptorSets(commandBuffers[index], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineSet->mPipelineLayout, 0, 1, &descriptorSet, 0, nullptr);

                VkBuffer vertexBuffers[] = { drawableData->mVertexAndIndexBuffer->mBuffer->GetVkBuffer() };
                VkDeviceSize offsets[] = { 0 };
                VulkanInterface::vkCmdBindVertexBuffers(commandBuffers[index], 0, 1, vertexBuffers, offsets);

                VkDeviceSize indicesOffset = drawableData->mVertexAndIndexBuffer->mOffsetToIndexBuffer;
                VulkanInterface::vkCmdBindIndexBuffer(commandBuffers[index], vertexBuffers[0], indicesOffset, VK_INDEX_TYPE_UINT32);

                uint32_t indicesSize = drawableData->mVertexAndIndexBuffer->mIndexBufferSize;
                VulkanInterface::vkCmdDrawIndexed(commandBuffers[index], indicesSize, 1, 0, 0, 0);
            }

        }

        checkVk(
            VulkanInterface::vkEndCommandBuffer(commandBuffers[index]),
            "VreCommandBuffer::recordCommandBuffer: failed to record command buffer!"
        );
    }

    void VreCommandBuffer::allocateCommandBuffers(std::vector<VkCommandBuffer>& commandBuffers, VkCommandBufferLevel cmdBufferLevel)
    {
        VkDevice device = VulkanManager::GetInstance()->GetDevice();
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = VreRenderer::GetInstance()->GetVreCommandPool()->GetCommandPool();
        allocInfo.level = cmdBufferLevel;
        allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

        checkVk(
            VulkanInterface::vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()),
            "RttCommandBuffer::allocateCommandBuffers: failed to allocate command buffers!"
        );
    }

    void VreCommandBuffer::setupRenderPassInfo()
    {
        // setup clear values by render target info
        auto info = m_vreRenderList->GetRenderTargetInfo();
        m_clearValues[0].color = {
            info->mClearValuesColor[0],
            info->mClearValuesColor[1], 
            info->mClearValuesColor[2],
            info->mClearValuesColor[3]
        };
        m_clearValues[1].depthStencil = { info->mClearValuesDepth, info->mClearValuesStencil };

        // setup render pass info with constant data
        m_renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        m_renderPassBeginInfo.renderPass = m_vreRenderList->GetVreRenderPass()->GetRenderPass();
        m_renderPassBeginInfo.renderArea.offset = { 0, 0 };
        m_renderPassBeginInfo.renderArea.extent.height = info->mHeight;
        m_renderPassBeginInfo.renderArea.extent.width = info->mWidth;
        m_renderPassBeginInfo.clearValueCount = static_cast<uint32_t>(m_clearValues.size());
        m_renderPassBeginInfo.pClearValues = m_clearValues.data();

        // link framebuffers
        m_framebuffers = m_vreRenderList->GetVreFrameBuffer()->GetFrameBuffers();
    }
}