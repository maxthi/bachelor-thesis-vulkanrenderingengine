#pragma once
#include "../common.hpp"
#include "../../dependencies/vk_mem_alloc/vk_mem_alloc.h"

namespace vre
{
	class VulkanMemoryManager;

	/// @brief Struct containing information whether to do an image layout transition and how to perform it.
	struct ImageTransition {
		bool mDoTransition{ true };
		VkFormat mFormat{ VK_FORMAT_R8G8B8A8_SRGB };
		VkImageLayout mOldLayout{ VK_IMAGE_LAYOUT_UNDEFINED };
		VkImageLayout mNewLayout{ VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL };
		bool mHasStencilComponent{ false };
	};

	/// @brief Struct containing info whether to automatically create an image view along with the image.
	/// Holds information about the image view to create.
	struct ImageView {
		bool mCreateImageView{ false };
		VkImageAspectFlags mAspectMask{ VK_IMAGE_ASPECT_COLOR_BIT };
		VkImageViewType mImageViewType{ VK_IMAGE_VIEW_TYPE_2D };
		uint32_t mBaseMipLevel{ 0 };
	};

	/// @brief Paramter to be passed to the Image constructor. Holds information about the image to be allocated.
	/// Most paramters correspond directly to the vulkan paramaters. The required parameters are mHeight, mWidth and
	/// mUsageFlags. If you want the image to be filled with data, set mImageSize and mImageData.
	/// Set mImageTransition to specify the image transition and mImageView to specify if an image view should be created.
	struct ImageInfo
	{
		// these are required to be set for every image
		uint32_t mHeight{ 0 };
		uint32_t mWidth{ 0 };
		VkImageUsageFlags mUsageFlags{ 0 };

		// size & data are optinal to be set
		// if they are set, the functions will try to use
		// either memory mapping or a staging buffer to transfer the data into the image memory
		size_t mImageSize{ 0 };
		const void* mImageData{ nullptr };

		// more detailed info about image creation, which can usually be left to its default values
		ImageTransition mImageTransition{};
		uint32_t mDepth{ 1 };
		uint32_t mMipLevels{ 1 };
		uint32_t mArrayLayers{ 1 };
		VkImageType mImageType{ VK_IMAGE_TYPE_2D };
		VkFormat mFormat{ VK_FORMAT_R8G8B8A8_SRGB };
		VkImageTiling mTiling{ VK_IMAGE_TILING_OPTIMAL };
		VkImageLayout mInitialLayout{ VK_IMAGE_LAYOUT_UNDEFINED };
		VkSharingMode mSharingMode{ VK_SHARING_MODE_EXCLUSIVE };
		VkSampleCountFlagBits mSampleCountFlagBits{ VK_SAMPLE_COUNT_1_BIT };
		VmaMemoryUsage mVmaMemoryUsage{ VMA_MEMORY_USAGE_CPU_ONLY };

		// info about creating related image view
		ImageView mImageView;

		/// @brief Name of the allocated image. Only for debugging purpose to keep track of an image.
		std::string mImageName{""};
		
	};

	/// @brief Helper class to allocate memory on the GPU to be used for images. Do not call these directly,
	/// use the vulkan memory manager class instead that can be accessed via the vulkan manager.
	class Image
	{
	public:
		Image(VulkanMemoryManager* pVulkanMemoryManager, const ImageInfo& info);
		~Image();

		/// @brief Map memory to the allocated memory. Only works with cpu mappable images.
		/// @param size - Size in bytes to be copied.
		/// @param data - Pointer to the data to be copied.
		void MapMemory(size_t size, const void* data);

		/// @brief Get the vk handle to the allocated image.
		VkImage GetVkImage();

		/// @brief Get vk handle to the image view.
		VkImageView GetImageView();

		/// @brief Get the format used to create the image.
		VkFormat GetImageFormat();

	private:
		/// @brief Number of allocated images. Just for information.
		static inline uint32_t s_imageCount{ 0 };

		VulkanMemoryManager* m_pVulkanMemoryManager{ nullptr };

		/// @brief Total size in bytes of the image.
		size_t m_size{};

		/// @brief Pointer to the vulkan image handle.
		VkImage m_image{};

		/// @brief  Pointer to the vulkan image view handle.
		VkImageView m_imageView{};

		/// @brief  The allocation created by vma.
		VmaAllocation m_allocation{};

		/// @brief Imageformat.
		VkFormat m_imageFormat;

		/// @brief Whether the allocated memory can be memory mapped.
		bool m_isMemoryMappable{ false };

		/// @brief Name of the image. Useful for tracking the image while debugging.
		std::string m_imageName{""};
	};
}
