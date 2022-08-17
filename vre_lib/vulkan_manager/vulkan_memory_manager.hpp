#pragma once
#include "../common.hpp"
#include "../../dependencies/vk_mem_alloc/vk_mem_alloc.h"

namespace vre
{
	class VulkanManager;
	class Buffer;
	class Image;
	struct ImageInfo;
	struct ImageTransition;
	struct BufferInfo;

	class VulkanMemoryManager
	{
	public:
		VulkanMemoryManager();
		~VulkanMemoryManager();

		VmaAllocator GetAllocator();

		/// @brief Create a vulkan buffer on the GPU that can be mapped directly from the CPU by either providing 
		/// a pointer to valid data in the BufferInfo structure or by using the MapMemory method.
		/// @param bufferInfo - BufferInfo containing the parameters for the buffer creation.
		std::shared_ptr<Buffer> CreateBufferMemoryMapped(BufferInfo& bufferInfo);

		/// @brief Create a buffer on the GPU that cannot be mapped directly. If a valid data pointer is provided in the BufferInfo structure,
		/// a staging buffer is created automatically to transfer the data to the GPU.
		/// @param bufferInfo - BufferInfo containing the parameters for the buffer creation.
		std::shared_ptr<Buffer> CreateBufferDeviceLocal(BufferInfo& bufferInfo);

		// CPU mappable image
		std::shared_ptr<Image> CreateImageMemoryMapped(ImageInfo& imageInfo);

		// uses staging buffer to copy into GPU local memory
		std::shared_ptr<Image> CreateImageDeviceLocal(ImageInfo& imageInfo);

		VkDevice GetDevice();

		/// @brief Called by the buffer constructor adding a buffer to the list of allocations.
		/// @param buffer - Pointer to the newly allocated buffer.
		/// @param bufferName - A name for identification.
		void AddBufferAllocation(Buffer* buffer, std::string bufferName);

		/// @brief Remove a buffer of the buffer allocations list, called by the buffer class destructor.
		/// @param buffer - Pointer to the buffer to remove
		void RemoveBufferAllocation(Buffer* buffer);

		/// @brief Called by the image constructor adding an image to the list of allocations.
		/// @param image - Pointer to the allocated image.
		/// @param imageName - A name of the image for identification.
		void AddImageAllocation(Image* image, std::string imageName);

		/// @brief Remove an image of the image allocations list, called by the image destructor.
		/// @param image - Pointer of the image to remove.
		void RemoveImageAllocation(Image* image);

		/// @brief Checks if all allocations have been freed. If not, will throw an error containing
		/// the names of all allocated resources.
		void CheckAllocations();

	private:
		VmaAllocator m_vmaAllocator{};

		std::unordered_map<Buffer*, std::string> m_bufferAllocations;
		std::unordered_map<Image*, std::string> m_imageAllocations;

		void copyBuffer(Buffer& srcBuffer, Buffer& dstBuffer);
		void transitionImageLayout(VkImage image, ImageTransition transition);
		void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

	};
}
