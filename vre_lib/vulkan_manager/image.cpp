#include "image.hpp"
#include "../vulkan_manager/vulkan_manager.hpp"

namespace vre
{
	Image::Image(VulkanMemoryManager* pVulkanMemoryManager, const ImageInfo& info) :
		m_pVulkanMemoryManager(pVulkanMemoryManager)
	{
		m_imageFormat = info.mFormat;

		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = info.mImageType;
		imageInfo.extent.width = info.mWidth;
		imageInfo.extent.height = info.mHeight;
		imageInfo.extent.depth = info.mDepth;
		imageInfo.mipLevels = info.mMipLevels;
		imageInfo.arrayLayers = info.mArrayLayers;
		imageInfo.format = info.mFormat;
		imageInfo.tiling = info.mTiling;
		imageInfo.initialLayout = info.mInitialLayout;
		imageInfo.usage = info.mUsageFlags;
		imageInfo.sharingMode = info.mSharingMode;
		imageInfo.samples = info.mSampleCountFlagBits;

		VmaAllocator allocator = pVulkanMemoryManager->GetAllocator();

		VmaAllocationCreateInfo allocInfo = {};
		allocInfo.usage = info.mVmaMemoryUsage;

		vmaCreateImage(allocator, &imageInfo, &allocInfo, &m_image, &m_allocation, nullptr);

		m_isMemoryMappable = info.mVmaMemoryUsage == VMA_MEMORY_USAGE_CPU_ONLY || info.mVmaMemoryUsage == VMA_MEMORY_USAGE_CPU_TO_GPU;
		if (info.mImageData != nullptr && m_isMemoryMappable)
		{
			void* mappedData;
			vmaMapMemory(allocator, m_allocation, &mappedData);
			memcpy(mappedData, info.mImageData, info.mImageSize);
			vmaUnmapMemory(allocator, m_allocation);
		}

		// creating an image view is optional
		// using the same format as base image atm
		if (info.mImageView.mCreateImageView)
		{
			VkImageViewCreateInfo viewInfo{};
			viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			viewInfo.image = m_image;
			viewInfo.viewType = info.mImageView.mImageViewType;
			viewInfo.format = info.mFormat;
			viewInfo.subresourceRange.aspectMask = info.mImageView.mAspectMask;
			viewInfo.subresourceRange.baseMipLevel = info.mImageView.mBaseMipLevel;
			viewInfo.subresourceRange.levelCount = info.mMipLevels;
			viewInfo.subresourceRange.baseArrayLayer = 0;
			viewInfo.subresourceRange.layerCount = info.mArrayLayers;

			VkDevice device = m_pVulkanMemoryManager->GetDevice();
			checkVk(
				VulkanInterface::vkCreateImageView(device, &viewInfo, nullptr, &m_imageView),
				"Texture::createImageViews(): failed to create texture image view!"
			);
		}

		s_imageCount++;
		m_imageName = info.mImageName + std::string("_imageallocation_nr_") + std::to_string(s_imageCount);
		m_pVulkanMemoryManager->AddImageAllocation(this, m_imageName);
	}

	Image::~Image()
	{
		m_pVulkanMemoryManager->RemoveImageAllocation(this);
		VmaAllocator allocator = m_pVulkanMemoryManager->GetAllocator();
		vmaDestroyImage(allocator, m_image, m_allocation);
		if (m_imageView != nullptr)
		{
			VkDevice device = m_pVulkanMemoryManager->GetDevice();
			VulkanInterface::vkDestroyImageView(device, m_imageView, nullptr);
		}
	}

	void Image::MapMemory(size_t size, const void* data)
	{
		if (!m_isMemoryMappable) {
			err("Image::MapMemory: Image is not memory mapped!");
		}
		VmaAllocator allocator = m_pVulkanMemoryManager->GetAllocator();
		void* mappedData;
		vmaMapMemory(allocator, m_allocation, &mappedData);
		memcpy(mappedData, data, size);
		vmaUnmapMemory(allocator, m_allocation);
	}

	VkImage Image::GetVkImage()
	{
		return m_image;
	}

	VkImageView Image::GetImageView()
	{
		return m_imageView;
	}

	VkFormat Image::GetImageFormat()
	{
		return m_imageFormat;
	}
}
