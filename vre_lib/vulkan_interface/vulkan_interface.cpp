#include "vulkan_interface.hpp"

namespace vre::VulkanInterface
{
	// static member initialisation
	VulkanInterface VulkanInterface::VulkanInterface::Interface;

	// private constructor
	VulkanInterface::VulkanInterface() :
		m_pQVulkanFunctions(nullptr),
		m_pQVulkanDeviceFunctions(nullptr)
	{
	}

	// QVulkan setter
	void VulkanInterface::SetQVulkanFunctions(QVulkanFunctions* pQVulkanFunctions)
	{
		m_pQVulkanFunctions = pQVulkanFunctions;
	}

	void VulkanInterface::SetQVulkanDeviceFunctions(QVulkanDeviceFunctions* pQVulkanDeviceFunctions)
	{
		m_pQVulkanDeviceFunctions = pQVulkanDeviceFunctions;
	}

	// QVulkanFunctions
	VkResult vkEnumeratePhysicalDevices(VkInstance instance, uint32_t* pPhysicalDeviceCount, VkPhysicalDevice* pPhysicalDevices)
	{
		return VulkanInterface::Interface.GetQVulkanFunctions()->vkEnumeratePhysicalDevices(instance, pPhysicalDeviceCount, pPhysicalDevices);
	}

	void vkGetPhysicalDeviceProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties* pProperties)
	{
		return VulkanInterface::Interface.GetQVulkanFunctions()->vkGetPhysicalDeviceProperties(physicalDevice, pProperties);
	}

	void vkGetPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties* pQueueFamilyProperties)
	{
		return VulkanInterface::Interface.GetQVulkanFunctions()->vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
	}

	void vkGetPhysicalDeviceMemoryProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties* pMemoryProperties)
	{
		return VulkanInterface::Interface.GetQVulkanFunctions()->vkGetPhysicalDeviceMemoryProperties(physicalDevice, pMemoryProperties);
	}

	void vkGetPhysicalDeviceFeatures(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures* pFeatures)
	{
		return VulkanInterface::Interface.GetQVulkanFunctions()->vkGetPhysicalDeviceFeatures(physicalDevice, pFeatures);
	}

	void vkGetPhysicalDeviceFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties* pFormatProperties)
	{
		return VulkanInterface::Interface.GetQVulkanFunctions()->vkGetPhysicalDeviceFormatProperties(physicalDevice, format, pFormatProperties);
	}

	VkResult vkGetPhysicalDeviceImageFormatProperties(
		VkPhysicalDevice physicalDevice,
		VkFormat format,
		VkImageType type,
		VkImageTiling tiling,
		VkImageUsageFlags usage,
		VkImageCreateFlags flags,
		VkImageFormatProperties* pImageFormatProperties
	)
	{
		return VulkanInterface::Interface.GetQVulkanFunctions()->vkGetPhysicalDeviceImageFormatProperties(
			physicalDevice,
			format,
			type,
			tiling,
			usage, 
			flags,
			pImageFormatProperties
		);
	}

	VkResult vkCreateDevice(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice)
	{
		return VulkanInterface::Interface.GetQVulkanFunctions()->vkCreateDevice(physicalDevice, pCreateInfo, pAllocator, pDevice);
	}

	VkResult vkEnumerateInstanceLayerProperties(uint32_t* pPropertyCount, VkLayerProperties* pProperties)
	{
		return VulkanInterface::Interface.GetQVulkanFunctions()->vkEnumerateInstanceLayerProperties(pPropertyCount, pProperties);
	}

	VkResult vkEnumerateInstanceExtensionProperties(const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties)
	{
		return VulkanInterface::Interface.GetQVulkanFunctions()->vkEnumerateInstanceExtensionProperties(pLayerName, pPropertyCount, pProperties);
	}

	VkResult vkEnumerateDeviceLayerProperties(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkLayerProperties* pProperties)
	{
		return VulkanInterface::Interface.GetQVulkanFunctions()->vkEnumerateDeviceLayerProperties(physicalDevice, pPropertyCount, pProperties);
	}

	VkResult vkEnumerateDeviceExtensionProperties(VkPhysicalDevice physicalDevice, const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties)
	{
		return VulkanInterface::Interface.GetQVulkanFunctions()->vkEnumerateDeviceExtensionProperties(physicalDevice, pLayerName, pPropertyCount, pProperties);
	}

	void vkGetPhysicalDeviceSparseImageFormatProperties(
		VkPhysicalDevice physicalDevice,
		VkFormat format,
		VkImageType type,
		VkSampleCountFlagBits samples,
		VkImageUsageFlags usage,
		VkImageTiling tiling,
		uint32_t* pPropertyCount,
		VkSparseImageFormatProperties* pProperties
	)
	{
		return VulkanInterface::Interface.GetQVulkanFunctions()->vkGetPhysicalDeviceSparseImageFormatProperties(
			physicalDevice,
			format,
			type,
			samples,
			usage,
			tiling,
			pPropertyCount,
			pProperties
		);
	}

	// QVulkanDeviceFunctions
	void vkDestroyDevice(VkDevice device, const VkAllocationCallbacks* pAllocator)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkDestroyDevice(device, pAllocator);
	}

	void vkGetDeviceQueue(VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkGetDeviceQueue(device, queueFamilyIndex, queueIndex, pQueue);
	}

	VkResult vkQueueSubmit(VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkQueueSubmit(queue, submitCount, pSubmits, fence);
	}

	VkResult vkQueueWaitIdle(VkQueue queue)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkQueueWaitIdle(queue);
	}

	VkResult vkDeviceWaitIdle(VkDevice device)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkDeviceWaitIdle(device);
	}

	VkResult vkAllocateMemory(VkDevice device, const VkMemoryAllocateInfo* pAllocateInfo, const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkAllocateMemory(device, pAllocateInfo, pAllocator, pMemory);
	}

	void vkFreeMemory(VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks* pAllocator)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkFreeMemory(device, memory, pAllocator);
	}

	VkResult vkMapMemory(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkMapMemory(device, memory, offset, size, flags, ppData);
	}

	void vkUnmapMemory(VkDevice device, VkDeviceMemory memory)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkUnmapMemory(device, memory);
	}

	VkResult vkFlushMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkFlushMappedMemoryRanges(device, memoryRangeCount, pMemoryRanges);
	}

	VkResult vkInvalidateMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkInvalidateMappedMemoryRanges(device, memoryRangeCount, pMemoryRanges);
	}

	void vkGetDeviceMemoryCommitment(VkDevice device, VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkGetDeviceMemoryCommitment(device, memory, pCommittedMemoryInBytes);
	}

	void vkGetBufferMemoryRequirements(VkDevice device, VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkGetBufferMemoryRequirements(device, buffer, pMemoryRequirements);
	}

	VkResult vkBindBufferMemory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkBindBufferMemory(device, buffer, memory, memoryOffset);
	}

	void vkGetImageMemoryRequirements(VkDevice device, VkImage image, VkMemoryRequirements* pMemoryRequirements)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkGetImageMemoryRequirements(device, image, pMemoryRequirements);
	}

	VkResult vkBindImageMemory(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkBindImageMemory(device, image, memory, memoryOffset);
	}

	void vkGetImageSparseMemoryRequirements(VkDevice device, VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkGetImageSparseMemoryRequirements(device, image, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
	}

	VkResult vkQueueBindSparse(VkQueue queue, uint32_t bindInfoCount, const VkBindSparseInfo* pBindInfo, VkFence fence)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkQueueBindSparse(queue, bindInfoCount, pBindInfo, fence);
	}

	VkResult vkCreateFence(VkDevice device, const VkFenceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCreateFence(device, pCreateInfo, pAllocator, pFence);
	}

	void vkDestroyFence(VkDevice device, VkFence fence, const VkAllocationCallbacks* pAllocator)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkDestroyFence(device, fence, pAllocator);
	}

	VkResult vkResetFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkResetFences(device, fenceCount, pFences);
	}

	VkResult vkGetFenceStatus(VkDevice device, VkFence fence)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkGetFenceStatus(device, fence);
	}

	VkResult vkWaitForFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkWaitForFences(device, fenceCount, pFences, waitAll, timeout);
	}

	VkResult vkCreateSemaphore(VkDevice device, const VkSemaphoreCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCreateSemaphore(device, pCreateInfo, pAllocator, pSemaphore);
	}

	void vkDestroySemaphore(VkDevice device, VkSemaphore semaphore, const VkAllocationCallbacks* pAllocator)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkDestroySemaphore(device, semaphore, pAllocator);
	}

	VkResult vkCreateEvent(VkDevice device, const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkEvent* pEvent)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCreateEvent(device, pCreateInfo, pAllocator, pEvent);
	}

	void vkDestroyEvent(VkDevice device, VkEvent event, const VkAllocationCallbacks* pAllocator)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkDestroyEvent(device, event, pAllocator);
	}

	VkResult vkGetEventStatus(VkDevice device, VkEvent event)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkGetEventStatus(device, event);
	}

	VkResult vkSetEvent(VkDevice device, VkEvent event)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkSetEvent(device, event);
	}

	VkResult vkResetEvent(VkDevice device, VkEvent event)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkResetEvent(device, event);
	}

	VkResult vkCreateQueryPool(VkDevice device, const VkQueryPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCreateQueryPool(device, pCreateInfo, pAllocator, pQueryPool);
	}

	void vkDestroyQueryPool(VkDevice device, VkQueryPool queryPool, const VkAllocationCallbacks* pAllocator)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkDestroyQueryPool(device, queryPool, pAllocator);
	}

	VkResult vkGetQueryPoolResults(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkGetQueryPoolResults(device, queryPool, firstQuery, queryCount, dataSize, pData, stride, flags);
	}

	VkResult vkCreateBuffer(VkDevice device, const VkBufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCreateBuffer(device, pCreateInfo, pAllocator, pBuffer);
	}

	void vkDestroyBuffer(VkDevice device, VkBuffer buffer, const VkAllocationCallbacks* pAllocator)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkDestroyBuffer(device, buffer, pAllocator);
	}

	VkResult vkCreateBufferView(VkDevice device, const VkBufferViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferView* pView)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCreateBufferView(device, pCreateInfo, pAllocator, pView);
	}

	void vkDestroyBufferView(VkDevice device, VkBufferView bufferView, const VkAllocationCallbacks* pAllocator)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkDestroyBufferView(device, bufferView, pAllocator);
	}

	VkResult vkCreateImage(VkDevice device, const VkImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImage* pImage)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCreateImage(device, pCreateInfo, pAllocator, pImage);
	}

	void vkDestroyImage(VkDevice device, VkImage image, const VkAllocationCallbacks* pAllocator)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkDestroyImage(device, image, pAllocator);
	}

	void vkGetImageSubresourceLayout(VkDevice device, VkImage image, const VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkGetImageSubresourceLayout(device, image, pSubresource, pLayout);
	}

	VkResult vkCreateImageView(VkDevice device, const VkImageViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImageView* pView)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCreateImageView(device, pCreateInfo, pAllocator, pView);
	}

	void vkDestroyImageView(VkDevice device, VkImageView imageView, const VkAllocationCallbacks* pAllocator)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkDestroyImageView(device, imageView, pAllocator);
	}

	VkResult vkCreateShaderModule(VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCreateShaderModule(device, pCreateInfo, pAllocator, pShaderModule);
	}

	void vkDestroyShaderModule(VkDevice device, VkShaderModule shaderModule, const VkAllocationCallbacks* pAllocator)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkDestroyShaderModule(device, shaderModule, pAllocator);
	}

	VkResult vkCreatePipelineCache(VkDevice device, const VkPipelineCacheCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCreatePipelineCache(device, pCreateInfo, pAllocator, pPipelineCache);
	}

	void vkDestroyPipelineCache(VkDevice device, VkPipelineCache pipelineCache, const VkAllocationCallbacks* pAllocator)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkDestroyPipelineCache(device, pipelineCache, pAllocator);
	}

	VkResult vkGetPipelineCacheData(VkDevice device, VkPipelineCache pipelineCache, size_t* pDataSize, void* pData)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkGetPipelineCacheData(device, pipelineCache, pDataSize, pData);
	}

	VkResult vkMergePipelineCaches(VkDevice device, VkPipelineCache dstCache, uint32_t srcCacheCount, const VkPipelineCache* pSrcCaches)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkMergePipelineCaches(device, dstCache, srcCacheCount, pSrcCaches);
	}

	VkResult vkCreateGraphicsPipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCreateGraphicsPipelines(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
	}

	VkResult vkCreateComputePipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkComputePipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCreateComputePipelines(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
	}

	void vkDestroyPipeline(VkDevice device, VkPipeline pipeline, const VkAllocationCallbacks* pAllocator)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkDestroyPipeline(device, pipeline, pAllocator);
	}

	VkResult vkCreatePipelineLayout(VkDevice device, const VkPipelineLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCreatePipelineLayout(device, pCreateInfo, pAllocator, pPipelineLayout);
	}

	void vkDestroyPipelineLayout(VkDevice device, VkPipelineLayout pipelineLayout, const VkAllocationCallbacks* pAllocator)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkDestroyPipelineLayout(device, pipelineLayout, pAllocator);
	}

	VkResult vkCreateSampler(VkDevice device, const VkSamplerCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSampler* pSampler)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCreateSampler(device, pCreateInfo, pAllocator, pSampler);
	}

	void vkDestroySampler(VkDevice device, VkSampler sampler, const VkAllocationCallbacks* pAllocator)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkDestroySampler(device, sampler, pAllocator);
	}

	VkResult vkCreateDescriptorSetLayout(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCreateDescriptorSetLayout(device, pCreateInfo, pAllocator, pSetLayout);
	}

	void vkDestroyDescriptorSetLayout(VkDevice device, VkDescriptorSetLayout descriptorSetLayout, const VkAllocationCallbacks* pAllocator)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkDestroyDescriptorSetLayout(device, descriptorSetLayout, pAllocator);
	}

	VkResult vkCreateDescriptorPool(VkDevice device, const VkDescriptorPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCreateDescriptorPool(device, pCreateInfo, pAllocator, pDescriptorPool);
	}

	void vkDestroyDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, const VkAllocationCallbacks* pAllocator)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkDestroyDescriptorPool(device, descriptorPool, pAllocator);
	}

	VkResult vkResetDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkResetDescriptorPool(device, descriptorPool, flags);
	}

	VkResult vkAllocateDescriptorSets(VkDevice device, const VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkAllocateDescriptorSets(device, pAllocateInfo, pDescriptorSets);
	}

	VkResult vkFreeDescriptorSets(VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkFreeDescriptorSets(device, descriptorPool, descriptorSetCount, pDescriptorSets);
	}

	void vkUpdateDescriptorSets(VkDevice device, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const VkCopyDescriptorSet* pDescriptorCopies)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkUpdateDescriptorSets(device, descriptorWriteCount, pDescriptorWrites, descriptorCopyCount, pDescriptorCopies);
	}

	VkResult vkCreateFramebuffer(VkDevice device, const VkFramebufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCreateFramebuffer(device, pCreateInfo, pAllocator, pFramebuffer);
	}

	void vkDestroyFramebuffer(VkDevice device, VkFramebuffer framebuffer, const VkAllocationCallbacks* pAllocator)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkDestroyFramebuffer(device, framebuffer, pAllocator);
	}

	VkResult vkCreateRenderPass(VkDevice device, const VkRenderPassCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCreateRenderPass(device, pCreateInfo, pAllocator, pRenderPass);
	}

	void vkDestroyRenderPass(VkDevice device, VkRenderPass renderPass, const VkAllocationCallbacks* pAllocator)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkDestroyRenderPass(device, renderPass, pAllocator);
	}

	void vkGetRenderAreaGranularity(VkDevice device, VkRenderPass renderPass, VkExtent2D* pGranularity)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkGetRenderAreaGranularity(device, renderPass, pGranularity);
	}

	VkResult vkCreateCommandPool(VkDevice device, const VkCommandPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCreateCommandPool(device, pCreateInfo, pAllocator, pCommandPool);
	}

	void vkDestroyCommandPool(VkDevice device, VkCommandPool commandPool, const VkAllocationCallbacks* pAllocator)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkDestroyCommandPool(device, commandPool, pAllocator);
	}

	VkResult vkResetCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkResetCommandPool(device, commandPool, flags);
	}

	VkResult vkAllocateCommandBuffers(VkDevice device, const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkAllocateCommandBuffers(device, pAllocateInfo, pCommandBuffers);
	}

	void vkFreeCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkFreeCommandBuffers(device, commandPool, commandBufferCount, pCommandBuffers);
	}

	VkResult vkBeginCommandBuffer(VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkBeginCommandBuffer(commandBuffer, pBeginInfo);
	}

	VkResult vkEndCommandBuffer(VkCommandBuffer commandBuffer)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkEndCommandBuffer(commandBuffer);
	}

	VkResult vkResetCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkResetCommandBuffer(commandBuffer, flags);
	}

	void vkCmdBindPipeline(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCmdBindPipeline(commandBuffer, pipelineBindPoint, pipeline);
	}

	void vkCmdSetViewport(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCmdSetViewport(commandBuffer, firstViewport, viewportCount, pViewports);
	}

	void vkCmdSetScissor(VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCmdSetScissor(commandBuffer, firstScissor, scissorCount, pScissors);
	}

	void vkCmdSetLineWidth(VkCommandBuffer commandBuffer, float lineWidth)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCmdSetLineWidth(commandBuffer, lineWidth);
	}

	void vkCmdSetDepthBias(VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCmdSetDepthBias(commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
	}

	void vkCmdSetBlendConstants(VkCommandBuffer commandBuffer, const float blendConstants[4])
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCmdSetBlendConstants(commandBuffer, blendConstants);
	}

	void vkCmdSetDepthBounds(VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCmdSetDepthBounds(commandBuffer, minDepthBounds, maxDepthBounds);
	}

	void vkCmdSetStencilCompareMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCmdSetStencilCompareMask(commandBuffer, faceMask, compareMask);
	}

	void vkCmdSetStencilWriteMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCmdSetStencilWriteMask(commandBuffer, faceMask, writeMask);
	}

	void vkCmdSetStencilReference(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCmdSetStencilReference(commandBuffer, faceMask, reference);
	}

	void vkCmdBindDescriptorSets(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCmdBindDescriptorSets(commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
	}

	void vkCmdBindIndexBuffer(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCmdBindIndexBuffer(commandBuffer, buffer, offset, indexType);
	}

	void vkCmdBindVertexBuffers(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCmdBindVertexBuffers(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets);
	}

	void vkCmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCmdDraw(commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
	}

	void vkCmdDrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCmdDrawIndexed(commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
	}

	void vkCmdDrawIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCmdDrawIndirect(commandBuffer, buffer, offset, drawCount, stride);
	}

	void vkCmdDrawIndexedIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCmdDrawIndexedIndirect(commandBuffer, buffer, offset, drawCount, stride);
	}

	void vkCmdDispatch(VkCommandBuffer commandBuffer, uint32_t x, uint32_t y, uint32_t z)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCmdDispatch(commandBuffer, x, y, z);
	}

	void vkCmdDispatchIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCmdDispatchIndirect(commandBuffer, buffer, offset);
	}

	void vkCmdCopyBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);
	}

	void vkCmdCopyImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCmdCopyImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
	}

	void vkCmdBlitImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCmdBlitImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter);
	}

	void vkCmdCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCmdCopyBufferToImage(commandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
	}

	void vkCmdCopyImageToBuffer(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCmdCopyImageToBuffer(commandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);
	}

	void vkCmdUpdateBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCmdUpdateBuffer(commandBuffer, dstBuffer, dstOffset, dataSize, pData);
	}

	void vkCmdFillBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCmdFillBuffer(commandBuffer, dstBuffer, dstOffset, size, data);
	}

	void vkCmdClearColorImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCmdClearColorImage(commandBuffer, image, imageLayout, pColor, rangeCount, pRanges);
	}

	void vkCmdClearDepthStencilImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCmdClearDepthStencilImage(commandBuffer, image, imageLayout, pDepthStencil, rangeCount, pRanges);
	}

	void vkCmdClearAttachments(VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCmdClearAttachments(commandBuffer, attachmentCount, pAttachments, rectCount, pRects);
	}

	void vkCmdResolveImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCmdResolveImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
	}

	void vkCmdSetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCmdSetEvent(commandBuffer, event, stageMask);
	}

	void vkCmdResetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCmdResetEvent(commandBuffer, event, stageMask);
	}

	void vkCmdWaitEvents(VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCmdWaitEvents(commandBuffer, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
	}

	void vkCmdPipelineBarrier(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCmdPipelineBarrier(commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
	}

	void vkCmdBeginQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCmdBeginQuery(commandBuffer, queryPool, query, flags);
	}

	void vkCmdEndQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCmdEndQuery(commandBuffer, queryPool, query);
	}

	void vkCmdResetQueryPool(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCmdResetQueryPool(commandBuffer, queryPool, firstQuery, queryCount);
	}

	void vkCmdWriteTimestamp(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCmdWriteTimestamp(commandBuffer, pipelineStage, queryPool, query);
	}

	void vkCmdCopyQueryPoolResults(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCmdCopyQueryPoolResults(commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags);
	}

	void vkCmdPushConstants(VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCmdPushConstants(commandBuffer, layout, stageFlags, offset, size, pValues);
	}

	void vkCmdBeginRenderPass(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCmdBeginRenderPass(commandBuffer, pRenderPassBegin, contents);
	}

	void vkCmdNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents contents)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCmdNextSubpass(commandBuffer, contents);
	}

	void vkCmdEndRenderPass(VkCommandBuffer commandBuffer)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCmdEndRenderPass(commandBuffer);
	}

	void vkCmdExecuteCommands(VkCommandBuffer commandBuffer, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers)
	{
		return VulkanInterface::Interface.GetQVulkanDeviceFunctions()->vkCmdExecuteCommands(commandBuffer, commandBufferCount, pCommandBuffers);
	}

	// KHR Extensions
	VkResult vkGetPhysicalDeviceSurfaceSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported)
	{
		return ::vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndex, surface, pSupported);
	}

	VkResult vkGetPhysicalDeviceSurfaceCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities)
	{
		return ::vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, pSurfaceCapabilities);
	}

	VkResult vkGetPhysicalDeviceSurfaceFormatsKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pSurfaceFormatCount, VkSurfaceFormatKHR* pSurfaceFormats)
	{
		return ::vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, pSurfaceFormatCount, pSurfaceFormats);
	}

	VkResult vkCreateSwapchainKHR(VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain)
	{
		return ::vkCreateSwapchainKHR(device, pCreateInfo, pAllocator, pSwapchain);
	}

	void vkDestroySwapchainKHR(VkDevice device, VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator)
	{
		::vkDestroySwapchainKHR(device, swapchain, pAllocator);
	}

	VkResult vkGetSwapchainImagesKHR(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages)
	{
		return ::vkGetSwapchainImagesKHR(device, swapchain, pSwapchainImageCount, pSwapchainImages);
	}

	VkResult vkAcquireNextImageKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex)
	{
		return ::vkAcquireNextImageKHR(device, swapchain, timeout, semaphore, fence, pImageIndex);
	}

	VkResult vkGetPhysicalDeviceSurfacePresentModesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes)
	{
		return ::vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, pPresentModeCount, pPresentModes);
	}
}