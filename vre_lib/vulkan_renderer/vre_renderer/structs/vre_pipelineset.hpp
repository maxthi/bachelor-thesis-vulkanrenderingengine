#pragma once
#include "../../../common.hpp"

namespace vre::Renderer::VreRenderer
{
	/// @brief Stores a pipeline and the according layout. Used for each render targets to keep track of the created
	/// pipelines.
	struct VrePipelineSet
	{
		~VrePipelineSet()
		{
			VkDevice device = VulkanManager::GetInstance()->GetDevice();
			if (mPipelineLayout != VK_NULL_HANDLE)
			{
				VulkanInterface::vkDestroyPipelineLayout(device, mPipelineLayout, nullptr);
			}
			if (mPipeline != VK_NULL_HANDLE)
			{
				VulkanInterface::vkDestroyPipeline(device, mPipeline, nullptr);
			}
		}

		/// @brief Pipeline object must be created by VreGraphicsPipielines.
		VkPipeline mPipeline{ VK_NULL_HANDLE };

		/// @brief The pipeline layout according to the pipeline.
		VkPipelineLayout mPipelineLayout{ VK_NULL_HANDLE };
	};

}