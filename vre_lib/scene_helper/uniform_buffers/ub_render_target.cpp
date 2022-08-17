#include "ub_render_target.hpp"
#include "../../scene_manager/drawable_properties/render_target.hpp"
#include <qmath.h>

namespace vre
{
	void UniformBufferRenderTarget::Create(std::shared_ptr<DrawableData> drawableData)
	{
		std::vector<VkDescriptorType> descriptorTypes = { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER };
		basicSetup(drawableData,
			sizeof(ModelViewProjectionUniformBufferObject),
			descriptorTypes,
			GetDescriptorSetLayout()->GetVkDescriptorSetLayout(),
			"UniformBufferRenderTarget"
		);

		size_t swapchainSize = VulkanManager::GetInstance()->GetSwapchainInfo()->size;

		std::shared_ptr<RenderTarget> renderTarget = drawableData->mNode->GetDrawable()->GetProperty<RenderTarget>();

		// make sure render target is allocated before using it
		renderTarget->AllocateMemory(drawableData);

		auto imageViews = renderTarget->GetImageViews();

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
			imageInfo.imageView = imageViews->at(i);
			imageInfo.sampler = VulkanManager::GetInstance()->GetTextureSampler();

			descriptorWrites[0].pBufferInfo = &bufferInfo;
			descriptorWrites[1].pImageInfo = &imageInfo;

			updateDescriptorSets(descriptorTypes, descriptorWrites, i);
		}

	}

	std::shared_ptr<DescriptorSetLayout> UniformBufferRenderTarget::GetDescriptorSetLayout()
	{
		return DescriptorSetLayoutTexture::GetLayout();
	}
}