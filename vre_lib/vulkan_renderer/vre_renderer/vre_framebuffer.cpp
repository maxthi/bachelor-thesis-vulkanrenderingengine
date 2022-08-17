#include "vre_framebuffer.hpp"
#include "vre_renderpass.hpp"
#include "vre_attachments.hpp"

namespace vre::Renderer::VreRenderer
{
    void VreFrameBuffer::Create()
    {
        VkDevice device = VulkanManager::GetInstance()->GetDevice();

        // image views
        auto imageViews = m_vreRenderList->GetRenderTargetInfo()->mImageViews;
        size_t imageCount = imageViews->size();

        // renderpass
        VkRenderPass renderpass = m_vreRenderList->GetVreRenderPass()->GetRenderPass();

        // color attachment
        auto msaaImage = m_vreRenderList->GetVreAttachments()->GetMSAAImage();
        VkImageView msaaImageView = msaaImage->GetImageView();

        // depth attachment
        auto depthImage = m_vreRenderList->GetVreAttachments()->GetDepthImage();
        VkImageView depthImageView = depthImage->GetImageView();

        // framebuffer dimensions
        uint32_t width = m_vreRenderList->GetRenderTargetInfo()->mWidth;
        uint32_t height = m_vreRenderList->GetRenderTargetInfo()->mHeight;

        m_framebuffers.resize(imageCount);
        for (size_t i = 0; i < imageCount; i++) {
            std::array<VkImageView, 3> attachments = {
                msaaImageView,
                depthImageView,
                imageViews->at(i),
            };

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = renderpass;
            framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
            framebufferInfo.pAttachments = attachments.data();
            framebufferInfo.width = width;
            framebufferInfo.height = height;
            framebufferInfo.layers = 1;

            checkVk(
                VulkanInterface::vkCreateFramebuffer(device, &framebufferInfo, nullptr, &m_framebuffers[i]),
                "VreFrameBuffer::VreFrameBuffer: failed to create framebuffer!"
            );
        }
    }

    void VreFrameBuffer::Destroy()
    {
        VkDevice device = VulkanManager::GetInstance()->GetDevice();
        for (auto framebuffer : m_framebuffers) {
            VulkanInterface::vkDestroyFramebuffer(device, framebuffer, nullptr);
            framebuffer = nullptr;
        }
    }

    const std::vector<VkFramebuffer>* VreFrameBuffer::GetFrameBuffers()
    {
        return &m_framebuffers;
    }
}