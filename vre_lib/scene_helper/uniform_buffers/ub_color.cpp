#include "ub_color.hpp"
#include "../../vulkan_manager/vulkan_manager.hpp"
#include "../../scene_manager/scene_manager.hpp"
#include <qmath.h>

namespace vre
{
	void UniformBufferColor::Create(std::shared_ptr<DrawableData> drawableData)
	{
		std::vector<VkDescriptorType> descriptorTypes = { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER };
		basicSetup(drawableData,
			sizeof(ModelViewProjectionUniformBufferObject),
			descriptorTypes,
			GetDescriptorSetLayout()->GetVkDescriptorSetLayout(),
			"UniformBufferColor"
		);

		size_t swapchainSize = VulkanManager::GetInstance()->GetSwapchainInfo()->size;

		// configure the descriptor sets
		std::vector<DescriptorWrite> descriptorWrites(1);
		for (size_t i = 0; i < swapchainSize; i++) {
			// referencing the actual buffer
			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = m_uniformBuffers[i]->GetVkBuffer();
			bufferInfo.offset = 0;
			bufferInfo.range = sizeof(ModelViewProjectionUniformBufferObject);

			descriptorWrites[0].pBufferInfo = &bufferInfo;
			
			updateDescriptorSets(descriptorTypes, descriptorWrites, i);
		}

	}

	std::shared_ptr<DescriptorSetLayout> UniformBufferColor::GetDescriptorSetLayout()
	{
		return DescriptorSetLayoutColor::GetLayout();
	}
}