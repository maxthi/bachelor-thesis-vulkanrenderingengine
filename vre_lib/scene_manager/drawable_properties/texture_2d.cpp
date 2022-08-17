#include "texture_2d.hpp"

namespace vre
{
	Texture2D::Texture2D(const std::string& imagePath)
	{
		m_propertyId = GetClassPropertyId();

		m_imagePath = Filesystem::validateFilePath(imagePath);

		if (!m_image.load(m_imagePath.c_str()))
		{
			err(std::string("Texture2D::Texture2D: failed to load texture image for path: ") + m_imagePath);
			return;
		}

		m_width = m_image.width();
		m_height = m_image.height();
		if (m_image.format() != QImage::Format::Format_RGBA8888)
		{
			m_image = m_image.convertToFormat(QImage::Format::Format_RGBA8888);
		}
		m_data = m_image.bits();
	}

	Texture2D::~Texture2D()
	{
		if (!m_vulkanImage)
		{
			return;
		}
		m_vulkanImage.reset();
	}

	std::shared_ptr<Image> Texture2D::GetImage()
	{
		if (!m_vulkanImage)
		{
			err("Texture2D::GetImage(): Attempted to get image before it was allocated!");
		}

		return m_vulkanImage;
	}

	void Texture2D::AllocateMemory(std::shared_ptr<DrawableData>& drawableData)
	{
		if (m_vulkanImage)
		{
			return;
		}

		ImageInfo imageInfo{};
		imageInfo.mImageView.mCreateImageView = true;
		imageInfo.mImageName = "2DTexture";
		VkDeviceSize imageSize = m_width * m_height * 4;
		imageInfo.mWidth = m_width;
		imageInfo.mHeight = m_height;
		imageInfo.mUsageFlags = VK_IMAGE_USAGE_SAMPLED_BIT;
		imageInfo.mImageSize = imageSize;
		imageInfo.mImageData = m_data;
		imageInfo.mImageTransition.mOldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		imageInfo.mImageTransition.mNewLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		m_vulkanImage = VulkanManager::GetInstance()->CreateImageDeviceLocal(imageInfo);
	}
}