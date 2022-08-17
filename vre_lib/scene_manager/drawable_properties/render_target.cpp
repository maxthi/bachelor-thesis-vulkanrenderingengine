#include "render_target.hpp"
#include "../../vulkan_manager/vulkan_manager.hpp"

namespace vre
{
	RenderTarget::RenderTarget(std::shared_ptr<RenderTargetConfiguration> config) :
		m_config(config)
	{
	}

	void RenderTarget::AllocateMemory(std::shared_ptr<DrawableData>& drawableData)
	{
		if (m_textures.size() > 0)
		{
			return;
		}

		auto vulkanManager = VulkanManager::GetInstance();
		const vre::SwapchainInfo* info = vulkanManager->GetSwapchainInfo();
		m_textures.reserve(info->size);

		vre::ImageInfo imgInfo{};
		imgInfo.mHeight = m_config->mHeight;
		imgInfo.mWidth = m_config->mWidth;
		imgInfo.mUsageFlags = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		imgInfo.mImageView.mCreateImageView = true;
		imgInfo.mFormat = m_config->mFormat;
		imgInfo.mImageTransition.mNewLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		for (size_t i = 0; i < info->size; i++)
		{
			imgInfo.mImageName = "RenderTargetTexture" + std::to_string(i) + "_" + m_config->mName;
			auto image = vulkanManager->CreateImageDeviceLocal(imgInfo);
			m_textures.push_back(image);
			m_imageViews.push_back(image->GetImageView());
		}

	}

	RenderTarget::~RenderTarget()
	{
		if (m_textures.size() == 0)
		{
			return;
		}

		for (size_t i = 0; i < m_textures.size(); i++)
		{
			m_textures[i].reset();
		}

		m_textures.clear();
		m_imageViews.clear();
	}
}