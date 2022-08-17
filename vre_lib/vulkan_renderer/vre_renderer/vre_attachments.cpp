#include "vre_attachments.hpp"
#include "../renderer.hpp"
#include "vre_renderlist.hpp"

namespace vre::Renderer::VreRenderer
{
	void VreAttachments::Create()
	{
		createDepthBuffer();
		createMSAABuffer();
	}

	void VreAttachments::Destroy()
	{
		destroyDepthBuffer();
		destroyMSAABuffer();
	}

	std::shared_ptr<Image> VreAttachments::GetDepthImage()
	{
		return m_depthImage;
	}

	std::shared_ptr<Image> VreAttachments::GetMSAAImage()
	{
		return m_msaaImage;
	}

	void VreAttachments::createDepthBuffer()
	{
		static VkFormat depthFormat = findDepthFormat();

		ImageInfo imageInfo{};
		imageInfo.mWidth = m_vreRenderList->GetRenderTargetInfo()->mWidth;
		imageInfo.mHeight = m_vreRenderList->GetRenderTargetInfo()->mHeight;
		imageInfo.mUsageFlags = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		imageInfo.mFormat = depthFormat;
		imageInfo.mSampleCountFlagBits = VulkanManager::GetInstance()->GetPhysicalDevice()->GetSampleCountFlagBits();
		imageInfo.mImageTransition.mFormat = depthFormat;
		imageInfo.mImageTransition.mHasStencilComponent = (depthFormat == VK_FORMAT_D32_SFLOAT_S8_UINT || depthFormat == VK_FORMAT_D24_UNORM_S8_UINT);
		imageInfo.mImageTransition.mNewLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		imageInfo.mImageName = "A DepthBuffer";

		imageInfo.mImageView.mCreateImageView = true;
		imageInfo.mImageView.mAspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		imageInfo.mImageView.mBaseMipLevel = 0;
		imageInfo.mImageView.mImageViewType = VK_IMAGE_VIEW_TYPE_2D;

		m_depthImage = VulkanManager::GetInstance()->CreateImageDeviceLocal(imageInfo);
	}

	void VreAttachments::destroyDepthBuffer()
	{
		m_depthImage.reset();
	}

	VkFormat VreAttachments::findDepthFormat()
	{
		return VulkanManager::GetInstance()->GetPhysicalDevice()->FindSupportedFormat(
			{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
			VK_IMAGE_TILING_OPTIMAL,
			VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
		);
	}

	void VreAttachments::createMSAABuffer()
	{
		ImageInfo imageInfo{};
		imageInfo.mWidth = m_vreRenderList->GetRenderTargetInfo()->mWidth;
		imageInfo.mHeight = m_vreRenderList->GetRenderTargetInfo()->mHeight;
		imageInfo.mUsageFlags = VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		imageInfo.mFormat = m_vreRenderList->GetRenderTargetInfo()->mFormat;
		imageInfo.mSampleCountFlagBits = VulkanManager::GetInstance()->GetPhysicalDevice()->GetSampleCountFlagBits();
		imageInfo.mImageTransition.mDoTransition = false;
		imageInfo.mImageName = "A MSAAImage";

		imageInfo.mImageView.mCreateImageView = true;
		imageInfo.mImageView.mAspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageInfo.mImageView.mBaseMipLevel = 0;
		imageInfo.mImageView.mImageViewType = VK_IMAGE_VIEW_TYPE_2D;

		m_msaaImage = VulkanManager::GetInstance()->CreateImageDeviceLocal(imageInfo);
	}

	void VreAttachments::destroyMSAABuffer()
	{
		m_msaaImage.reset();
	}
}