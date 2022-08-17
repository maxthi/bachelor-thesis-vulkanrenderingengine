#include "ub_texture.hpp"
#include "../../vulkan_manager/vulkan_manager.hpp"
#include "../../scene_manager/scene_manager.hpp"
#include <qmath.h>
namespace vre
{
	void UniformBufferTexture::Create(std::shared_ptr<DrawableData> drawableData)
	{
		std::vector<VkDescriptorType> descriptorTypes = { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER };
		basicSetup(drawableData,
			sizeof(ModelViewProjectionUniformBufferObject),
			descriptorTypes,
			GetDescriptorSetLayout()->GetVkDescriptorSetLayout(),
			"UniformBufferTexture"
		);

		size_t swapchainSize = VulkanManager::GetInstance()->GetSwapchainInfo()->size;

		// make sure texture is available before using it
		drawableData->mNode->GetDrawable()->GetProperty<Texture2D>()->AllocateMemory(drawableData);

		// configure the descriptor sets
		std::vector<DescriptorWrite> descriptorWrites(2);
		for (size_t i = 0; i < swapchainSize; i++) {
			// referencing the actual buffer
			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = m_uniformBuffers[i]->GetVkBuffer();
			bufferInfo.offset = 0;
			bufferInfo.range = sizeof(ModelViewProjectionUniformBufferObject);

			VkDescriptorImageInfo imageInfo{};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = drawableData->mNode->GetDrawable()->GetProperty<Texture2D>()->GetImage()->GetImageView();
			imageInfo.sampler = VulkanManager::GetInstance()->GetTextureSampler();

			descriptorWrites[0].pBufferInfo = &bufferInfo;
			descriptorWrites[1].pImageInfo = &imageInfo;

			updateDescriptorSets(descriptorTypes, descriptorWrites, i);
		}

	}

	std::shared_ptr<DescriptorSetLayout> UniformBufferTexture::GetDescriptorSetLayout()
	{
		return DescriptorSetLayoutTexture::GetLayout();
	}
}