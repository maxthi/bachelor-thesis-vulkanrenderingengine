#pragma once
#include "../common.hpp"

//#include <vk_mem_alloc/vk_mem_alloc.h>
#include "../../dependencies/vk_mem_alloc/vk_mem_alloc.h"

namespace vre
{
	class VulkanMemoryManager;

	struct BufferInfo
	{
		/// @brief Size of the buffer in bytes.
		size_t mDataSize{};

		/// @brief Pointer to the data to write to the buffer.
		const void* mData{ nullptr };

		/// @brief Vulkan buffer usage flags.
		VkBufferUsageFlags mUsageFlags{ 0 };

		/// @brief Vulkan memory allocator usage flags. See vma documentation.
		VmaMemoryUsage mVmaMemoryUsage{ VMA_MEMORY_USAGE_CPU_ONLY };

		VmaAllocationCreateFlags mVmaAllocationCreateFlags{};

		/// @brief Vulkan sharing mode.
		VkSharingMode mSharingMode{ VK_SHARING_MODE_EXCLUSIVE };

		/// @brief Name of the buffer. Only used for debug tracking purpose.
		std::string mName{ "" };
	};

	class Buffer
	{
	public:
		
		/// @brief Creates a buffer on the GPU. If the data ptr is not a null pointer, the memory will be mapped accordingly.
		/// @param pVulkanMemoryManager - A reference to the vulkan memory manager.
		/// @param size - The size of the buffer to be created.
		/// @param data - Either a nullptr or a pointer to data to be mapped.
		/// @param usageFlags - UsageFlags, see vulkan documentation.
		/// @param vmaMemoryUsage - Usage in terms of Vma, see vma documentation.
		/// @param sharingMode - Sharing mode of the buffer.
		//Buffer(VulkanMemoryManager* pVulkanMemoryManager, size_t size, const void* data, VkBufferUsageFlags usageFlags, VmaMemoryUsage vmaMemoryUsage = VMA_MEMORY_USAGE_CPU_ONLY, VkSharingMode sharingMode = VK_SHARING_MODE_EXCLUSIVE, std::string name = "");
		Buffer(VulkanMemoryManager* pVulkanMemoryManager, BufferInfo& bufferInfo);
		~Buffer();

		/// @brief Allows to write a certain amount of data directly to the buffer. This operation will fail if the buffer is
		/// not memory mappable.
		/// @param size - The number of bytes to be copied from data.
		/// @param data - A pointer to the data to be copied to the buffer.
		void MapMemory(size_t size, const void* data);

		/// @brief memcpys a block of memory to the gpu like MapMemory but assumes 
		/// that the memory is constantly mapped, so map/unmap does not need to be called.
		/// Only works if you used VMA_ALLOCATION_CREATE_MAPPED_BIT flag for creation.
		/// Many large persistenly mapped memory blocks may impact performance, see
		/// https://gpuopen-librariesandsdks.github.io/VulkanMemoryAllocator/html/memory_mapping.html
		/// @param size - Size of the memoryblock to be copied.
		/// @param data - Pointer to the data to be copied.
		void MapPersistentlyMappedMemory(size_t size, const void* data);

		/// @brief 
		/// @return The vulkan handle of the allocated buffer. 
		VkBuffer GetVkBuffer();

		/// @brief 
		/// @return The size of the buffer in bytes. 
		size_t GetSize();
	private:

		/// @brief Counts the number of buffer allocations.
		static inline uint32_t s_bufferCounter{ 0 };

		/// @brief Pointer to the vulkan memory manager.
		VulkanMemoryManager* m_pVulkanMemoryManager{ nullptr };

		/// @brief Name of the image. Can be used in debug mode to easily identify the resource.
		std::string m_imageName{ "" };

		/// @brief Stores the size of the allocated buffer in bytes.
		size_t m_size;

		/// @brief The vulkan handle to the buffer.
		VkBuffer m_buffer;

		/// @brief The associated vma allocation. Used for storing/freeing within the library of vma.
		VmaAllocation m_allocation;

		/// @brief Indicates if the memory can be written directly by the CPU.
		bool m_isMemoryMappable{ false };

		/// @brief The name of the buffer, for debugging purposes.
		std::string m_bufferName{""};

		VmaAllocator m_vmaAllocator{};

		VmaAllocationInfo m_vmaAllocationInfo{};
	};
}
