#include "buffer.hpp"
#include "vulkan_memory_manager.hpp"

namespace vre
{
	//Buffer::Buffer(VulkanMemoryManager* pVulkanMemoryManager, size_t size, const void* data, VkBufferUsageFlags usageFlags, VmaMemoryUsage vmaMemoryUsage, VkSharingMode sharingMode, std::string name) :
	Buffer::Buffer(VulkanMemoryManager* pVulkanMemoryManager, BufferInfo& bufferInfo) :
		m_pVulkanMemoryManager(pVulkanMemoryManager)
	{
		m_size = bufferInfo.mDataSize;
		m_vmaAllocator = pVulkanMemoryManager->GetAllocator();

		VkBufferCreateInfo bufferCreateInfo{};
		bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferCreateInfo.size = bufferInfo.mDataSize;
		bufferCreateInfo.usage = bufferInfo.mUsageFlags;
		bufferCreateInfo.sharingMode = bufferInfo.mSharingMode;

		VmaAllocationCreateInfo allocInfo = {};
		allocInfo.usage = bufferInfo.mVmaMemoryUsage;
		allocInfo.flags = bufferInfo.mVmaAllocationCreateFlags;

		vmaCreateBuffer(m_vmaAllocator, &bufferCreateInfo, &allocInfo, &m_buffer, &m_allocation, &m_vmaAllocationInfo);

		m_isMemoryMappable = bufferInfo.mVmaMemoryUsage == VMA_MEMORY_USAGE_CPU_ONLY || bufferInfo.mVmaMemoryUsage == VMA_MEMORY_USAGE_CPU_TO_GPU;
		if (bufferInfo.mData != nullptr && m_isMemoryMappable)
		{
			void* mappedData;
			vmaMapMemory(m_vmaAllocator, m_allocation, &mappedData);
			memcpy(mappedData, bufferInfo.mData, bufferInfo.mDataSize);
			vmaUnmapMemory(m_vmaAllocator, m_allocation);
		}

		s_bufferCounter++;
		m_bufferName = bufferInfo.mName + std::string("_bufferallocation_nr_") + std::to_string(s_bufferCounter);
		m_pVulkanMemoryManager->AddBufferAllocation(this, m_bufferName);
	}

	Buffer::~Buffer()
	{
		m_pVulkanMemoryManager->RemoveBufferAllocation(this);
		VmaAllocator allocator = m_pVulkanMemoryManager->GetAllocator();
		vmaDestroyBuffer(allocator, m_buffer, m_allocation);
	}

	void Buffer::MapMemory(size_t size, const void* data)
	{
		void* mappedData;
		vmaMapMemory(m_vmaAllocator, m_allocation, &mappedData);
		memcpy(mappedData, data, size);
		vmaUnmapMemory(m_vmaAllocator, m_allocation);
	}

	void Buffer::MapPersistentlyMappedMemory(size_t size, const void* data)
	{
		memcpy(m_vmaAllocationInfo.pMappedData, data, size);
	}

	VkBuffer Buffer::GetVkBuffer()
	{
		return m_buffer;
	}

	size_t Buffer::GetSize()
	{
		return m_size;
	}
}
