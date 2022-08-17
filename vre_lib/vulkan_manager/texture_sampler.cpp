#include "texture_sampler.hpp"
#include "vulkan_manager.hpp"

namespace vre
{
	TextureSampler::TextureSampler()
	{
		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
		samplerInfo.anisotropyEnable = VK_TRUE;
		samplerInfo.maxAnisotropy = 16.0f;
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.mipLodBias = 0.0f;
		samplerInfo.minLod = 0.0f;
		samplerInfo.maxLod = 0.0f;

		VkDevice device = VulkanManager::GetInstance()->GetDevice();
		checkVk(
			VulkanInterface::vkCreateSampler(device, &samplerInfo, nullptr, &m_sampler),
			"Texture::createSampler(): failed to create texture image view!"
		);
	}

	TextureSampler::~TextureSampler()
	{
		VkDevice device = VulkanManager::GetInstance()->GetDevice();
		VulkanInterface::vkDestroySampler(device, m_sampler, nullptr);
		m_sampler = VK_NULL_HANDLE;
	}


	VkSampler TextureSampler::GetSampler()
	{
		checkPtr(m_sampler, "TextureSampler::GetSampler: Sampler not created!");
		return m_sampler;
	}
}