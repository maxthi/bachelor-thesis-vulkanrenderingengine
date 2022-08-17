#include "uniform_buffer.hpp"
#include "../scene_manager/scene_objects/camera.hpp"
#include "../vulkan_manager/vulkan_manager.hpp"
#include "../scene_manager/scene_manager.hpp"

namespace vre
{
	UniformBuffer::UniformBuffer()
	{
		
	}

	void UniformBuffer::Destroy()
	{
		size_t uniformBufferSize = m_uniformBuffers.size();
		for (size_t i = 0; i < uniformBufferSize; i++)
		{
			m_uniformBuffers[i].reset();
		}

		VkDevice device = VulkanManager::GetInstance()->GetDevice();
		VulkanInterface::vkDestroyDescriptorPool(device, m_descriptorPool, nullptr);
	}

	std::shared_ptr<Buffer> UniformBuffer::GetUniformBuffer(uint32_t swapchainImageIndex)
	{
		return m_uniformBuffers[swapchainImageIndex];
	}

	void UniformBuffer::UpdateUniformBuffer(uint32_t swapchainImageIndex)
	{
		size_t size = sizeof(m_defaultUbo);
		memcpy(m_defaultUbo.model, m_transformationData, 64);
		memcpy(m_defaultUbo.view, m_view->data(), 64);
		memcpy(m_defaultUbo.proj, m_proj->data(), 64);
		m_uniformBuffers[swapchainImageIndex]->MapPersistentlyMappedMemory(size, &m_defaultUbo);
	}

	VkDescriptorSet UniformBuffer::GetDescriptorSet(uint32_t swapchainImageIndex)
	{
		return m_descriptorSets[swapchainImageIndex];
	}

	UniformBuffer::~UniformBuffer()
	{
		Destroy();
	}

	void UniformBuffer::basicSetup(
		std::shared_ptr<DrawableData> drawableData,
		VkDeviceSize uniformBufferSize,
		std::vector<VkDescriptorType>& descriptorTypes,
		VkDescriptorSetLayout descriptorSetLayout,
		std::string uniformBufferName
	)
	{
		m_drawableData = drawableData;
		m_transformationData = drawableData->mTransformation->data();

		// this part has to be renewed if the size of the swap changed
		size_t swapchainSize = VulkanManager::GetInstance()->GetSwapchainInfo()->size;
		m_uniformBuffers.resize(swapchainSize);

		BufferInfo bufferInfo{};
		bufferInfo.mDataSize = uniformBufferSize;
		bufferInfo.mUsageFlags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		bufferInfo.mVmaAllocationCreateFlags = VMA_ALLOCATION_CREATE_MAPPED_BIT;

		for (size_t i = 0; i < swapchainSize; i++)
		{
			std::string bufferName = uniformBufferName + std::to_string(i);
			bufferInfo.mName = bufferName;
			m_uniformBuffers[i] = VulkanManager::GetInstance()->CreateBufferMemoryMapped(bufferInfo);
		}

		// create descriptor pool
		std::vector<VkDescriptorPoolSize> poolSizes(descriptorTypes.size());
		for (size_t i = 0; i < descriptorTypes.size(); i++)
		{
			poolSizes[i].type = descriptorTypes[i];
			poolSizes[i].descriptorCount = static_cast<uint32_t>(swapchainSize);
		}

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());;
		poolInfo.pPoolSizes = poolSizes.data();
		poolInfo.maxSets = static_cast<uint32_t>(swapchainSize);

		VkDevice device = VulkanManager::GetInstance()->GetDevice();
		checkVk(
			VulkanInterface::vkCreateDescriptorPool(device, &poolInfo, nullptr, &m_descriptorPool),
			uniformBufferName + std::string("::Create: failed to create descriptor pool!")
		);

		// create the descriptor sets
		// a copy of the layout is needed for each set we create
		// one set per swapchain image is required
		std::vector<VkDescriptorSetLayout> layouts(swapchainSize, descriptorSetLayout);
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = m_descriptorPool;
		allocInfo.descriptorSetCount = static_cast<uint32_t>(swapchainSize);
		allocInfo.pSetLayouts = layouts.data();

		// allocated descriptor sets are destroyed when pool is
		m_descriptorSets.resize(swapchainSize);
		checkVk(
			// create all sets at once
			VulkanInterface::vkAllocateDescriptorSets(device, &allocInfo, m_descriptorSets.data()),
			uniformBufferName + std::string("::Create: failed to allocate descriptor sets!")
		);
	}

	void UniformBuffer::updateDescriptorSets(std::vector<VkDescriptorType>& descriptorTypes, std::vector<DescriptorWrite>& writes, size_t swapChainIndex)
	{
		// writing to the set
		std::vector<VkWriteDescriptorSet> descriptorWrites(descriptorTypes.size());

		for (size_t i = 0; i < descriptorTypes.size(); i++)
		{
			descriptorWrites[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[i].dstSet = m_descriptorSets[swapChainIndex];
			descriptorWrites[i].dstBinding = i;
			descriptorWrites[i].dstArrayElement = 0;
			descriptorWrites[i].descriptorType = descriptorTypes[i];
			descriptorWrites[i].descriptorCount = writes[i].descriptorCount;
			descriptorWrites[i].pBufferInfo = writes[i].pBufferInfo;
			descriptorWrites[i].pImageInfo = writes[i].pImageInfo;
		}

		VkDevice device = VulkanManager::GetInstance()->GetDevice();
		VulkanInterface::vkUpdateDescriptorSets(device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}

}