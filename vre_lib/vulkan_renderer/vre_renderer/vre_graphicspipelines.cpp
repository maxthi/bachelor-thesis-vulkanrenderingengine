#include "vre_graphicspipelines.hpp"
#include "vre_renderpass.hpp"
#include "../../shader_manager/shader_combination.hpp"
#include "../../shader_manager/shader_manager.hpp"


namespace vre::Renderer::VreRenderer
{
    void VreGraphicsPipelines::Create()
    {
        m_vreRenderList->ClearPipelineSets();

        auto vkConfig = VulkanManager::GetInstance()->GetVulkanConfiguration();

        float width = (float)m_vreRenderList->GetRenderTargetInfo()->mWidth;
        float height = (float)m_vreRenderList->GetRenderTargetInfo()->mHeight;
       
        // Viewports and scissors
        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = width;
        viewport.height = height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        // Note: Using a negative height and moving the y-axis changes the viewport to compensate for the difference of the Y axis between vulkan and openGL.
        // This allows to use the same shaders.
        // https://www.saschawillems.de/blog/2019/03/29/flipping-the-vulkan-viewport/
        if (vkConfig->UseRightHandedSystem)
        {
            viewport.y = height;
            viewport.height = -height;
        }

        VkExtent2D extent;
        extent.height = height;
        extent.width = width;

        VkRect2D scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = extent;

        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;

        // Multisampling
        vkConfig->mPipelineConfiguration.mPipelineMultisampleStateCreateInfo.rasterizationSamples = 
            VulkanManager::GetInstance()->GetPhysicalDevice()->GetSampleCountFlagBits();

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 1;
        pipelineLayoutInfo.pSetLayouts = nullptr; // set later for each pipeline
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;

        // creating the pipeline according to definitions
        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 0; // set later for each pipeline
        pipelineInfo.pStages = nullptr; // set later for each pipeline
        pipelineInfo.pVertexInputState = nullptr; // set later
        pipelineInfo.pInputAssemblyState = &vkConfig->mPipelineConfiguration.mPipelineInputAssemblyStateCreateInfo;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &vkConfig->mPipelineConfiguration.mPipelineRasterizationStateCreateInfo;
        pipelineInfo.pMultisampleState = &vkConfig->mPipelineConfiguration.mPipelineMultisampleStateCreateInfo;
        pipelineInfo.pColorBlendState = &vkConfig->mPipelineConfiguration.mPipelineColorBlendStateCreateInfo;
        pipelineInfo.pDepthStencilState = &vkConfig->mPipelineConfiguration.mPipelineDepthStencilStateCreateInfo;
        pipelineInfo.renderPass = m_vreRenderList->GetVreRenderPass()->GetRenderPass();
        pipelineInfo.subpass = 0;

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

        // create a pipeline for each shader combination used with the properties of this exact pipeline
        // and store it along with the drawables in the drawable list
        for (auto& drawableList : *m_vreRenderList->GetOpaqueDrawables())
        {
            createPipeline(drawableList, vertexInputInfo, pipelineLayoutInfo, pipelineInfo);
        }

        for (auto& drawableList : *m_vreRenderList->GetTransparentDrawables())
        {
            createPipeline(drawableList, vertexInputInfo, pipelineLayoutInfo, pipelineInfo);
        }
    }

    void VreGraphicsPipelines::createPipeline(
        DrawableList& drawableList,
        VkPipelineVertexInputStateCreateInfo& vertexInputInfo,
        VkPipelineLayoutCreateInfo& pipelineLayoutInfo,
        VkGraphicsPipelineCreateInfo& pipelineInfo
        )
    {
        auto shaderCombination = drawableList.mShaderCombination;
        VkDevice device = VulkanManager::GetInstance()->GetDevice();

        // cancel pipeline creation if a pipeline with the same shader combination id has already been created
        auto shaderCombinationIndex = shaderCombination->GetShaderCombinationIndex();
        auto pipelineSets = m_vreRenderList->GetPipelineSets();
        if (pipelineSets->find(shaderCombinationIndex) != pipelineSets->end())
        {
            return;
        }


        // update attribute and binding description for this pipeline
        auto bindingDescription = shaderCombination->GetVertexShader()->GetVertexLayout()->GetVertexInputBindingDescriptions();
        auto attributeDescriptions = shaderCombination->GetVertexShader()->GetVertexLayout()->GetVertexInputAttributeDescriptions();
        vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescription.size());;
        vertexInputInfo.pVertexBindingDescriptions = bindingDescription.data();
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

        // pipeline layout
        VkDescriptorSetLayout descriptorSetLayout = shaderCombination->GetDescriptorSetLayout()->GetVkDescriptorSetLayout();
        pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;
        auto descriptorSetLayoutObj = shaderCombination->GetDescriptorSetLayout();
        if (descriptorSetLayoutObj->UsesPushConstants())
        {
            pipelineLayoutInfo.pushConstantRangeCount = descriptorSetLayoutObj->GetPushConstantRangeCount();
            pipelineLayoutInfo.pPushConstantRanges = descriptorSetLayoutObj->GetPushConstantRanges();
        }
        VkPipelineLayout pipelineLayout;
        checkVk(
            VulkanInterface::vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout),
            "VreGraphicsPipelines::VreGraphicsPipelines: failed to create pipeline layout!"
        );

        // update pipeline info
        shaderCombination->CreateShaderModules();
        auto shaderStageCreateInfos = shaderCombination->GetShaderStageCreateInfos();
        pipelineInfo.stageCount = static_cast<uint32_t>(shaderStageCreateInfos.size());
        pipelineInfo.pStages = shaderStageCreateInfos.data();
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.layout = pipelineLayout;

        VkPipeline pipeline;
        checkVk(
            VulkanInterface::vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline),
            "VreGraphicsPipelines::VreGraphicsPipelines: failed to create graphics pipeline!"
        );
        shaderCombination->DestroyShaderModules();

        auto pipelineSet = std::make_shared<VrePipelineSet>();
        pipelineSet->mPipeline = pipeline;
        pipelineSet->mPipelineLayout = pipelineLayout;
        drawableList.mPipelineSet = pipelineSet;
        
        m_vreRenderList->AddPipelineSet(shaderCombinationIndex, pipelineSet);
    }

    void VreGraphicsPipelines::Destroy()
    {
    }
}