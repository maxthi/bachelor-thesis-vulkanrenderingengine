#include "ub_phong.hpp"
#include <qmath.h>

namespace vre
{
	void UniformBufferPhong::Create(std::shared_ptr<DrawableData> drawableData)
	{
		std::vector<VkDescriptorType> descriptorTypes = { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER };
		basicSetup(drawableData,
			sizeof(UniformBufferObject),
			descriptorTypes,
			GetDescriptorSetLayout()->GetVkDescriptorSetLayout(),
			"UniformBufferRenderTarget"
		);

		size_t swapchainSize = VulkanManager::GetInstance()->GetSwapchainInfo()->size;

		auto texture2D = drawableData->mNode->GetDrawable()->GetProperty<Texture2D>();
		texture2D->AllocateMemory(drawableData);

		// configure the descriptor sets
		std::vector<DescriptorWrite> descriptorWrites(2);
		for (size_t i = 0; i < swapchainSize; i++) {
			// referencing the actual buffer
			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = m_uniformBuffers[i]->GetVkBuffer();
			bufferInfo.offset = 0;
			bufferInfo.range = sizeof(UniformBufferObject);

			VkDescriptorImageInfo imageInfo{};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = texture2D->GetImage()->GetImageView();
			imageInfo.sampler = VulkanManager::GetInstance()->GetTextureSampler();

			descriptorWrites[0].pBufferInfo = &bufferInfo;
			descriptorWrites[1].pImageInfo = &imageInfo;

			updateDescriptorSets(descriptorTypes, descriptorWrites, i);
		}

	}

	void UniformBufferPhong::UpdateUniformBuffer(uint32_t swapchainImageIndex)
	{
		static UniformBufferObject ubo;

		// rotate by time
		static auto startTime = std::chrono::high_resolution_clock::now();
		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

		QMatrix4x4 light{};
		light.translate(10.0f * sin(time), 10.0f * cos(time), 0.0f);

		m_view = m_camera->GetViewMatrix();
		
		size_t size = sizeof(ubo);
		memcpy(ubo.model, m_transformationData, 64);
		memcpy(ubo.view, m_view->data(), 64);
		memcpy(ubo.proj, m_proj->data(), 64);
		auto lightPosition = light.column(3);
		memcpy(ubo.lightPosition, &lightPosition, 12);
		m_uniformBuffers[swapchainImageIndex]->MapPersistentlyMappedMemory(sizeof(ubo), &ubo);
	}

	std::shared_ptr<DescriptorSetLayout> UniformBufferPhong::GetDescriptorSetLayout()
	{
		return DescriptorSetLayoutTexture::GetLayout();
	}
}