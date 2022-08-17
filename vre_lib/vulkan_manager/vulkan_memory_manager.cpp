#include "vulkan_memory_manager.hpp"
#include "../vulkan_manager/vulkan_manager.hpp"
#include "buffer.hpp"
#include "image.hpp"

namespace vre
{
	VulkanMemoryManager::VulkanMemoryManager()
	{
		VmaAllocatorCreateInfo allocatorInfo{};
		allocatorInfo.physicalDevice = VulkanManager::GetInstance()->GetPhysicalDevice()->GetVkPhysicalDevice();
		allocatorInfo.device = VulkanManager::GetInstance()->GetDevice();
		allocatorInfo.instance = VulkanManager::GetInstance()->GetVulkanInstance()->GetVulkanInstance();

		vmaCreateAllocator(&allocatorInfo, &m_vmaAllocator);
	}

	VulkanMemoryManager::~VulkanMemoryManager()
	{
		CheckAllocations();
		vmaDestroyAllocator(m_vmaAllocator);
	}

	VmaAllocator VulkanMemoryManager::GetAllocator()
	{
		if (m_vmaAllocator == nullptr) {
			err("VulkanMemoryManager::GetAllocator: allocator is nullptr!");
		}
		return m_vmaAllocator;
	}

	std::shared_ptr<Buffer> VulkanMemoryManager::CreateBufferMemoryMapped(BufferInfo& bufferInfo)
	{
		return std::make_shared<Buffer>(this, bufferInfo);
	}

	std::shared_ptr<Buffer> VulkanMemoryManager::CreateBufferDeviceLocal(BufferInfo& bufferInfo)
	{
		bufferInfo.mUsageFlags |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		bufferInfo.mVmaMemoryUsage = VMA_MEMORY_USAGE_GPU_ONLY;
		std::shared_ptr<Buffer> deviceLocalBuffer = std::make_shared<Buffer>(this, bufferInfo);
		
		if (bufferInfo.mData != nullptr)
		{
			BufferInfo bufInfo{};
			bufInfo.mDataSize = bufferInfo.mDataSize;
			bufInfo.mData = bufferInfo.mData;
			bufInfo.mUsageFlags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
			Buffer stagingBuffer(this, bufInfo);
			copyBuffer(stagingBuffer, *deviceLocalBuffer);
		}
		return deviceLocalBuffer;
	}

	std::shared_ptr<Image> VulkanMemoryManager::CreateImageMemoryMapped(ImageInfo& imageInfo)
	{
		return std::make_shared<Image>(this, imageInfo);
	}

	std::shared_ptr<Image> VulkanMemoryManager::CreateImageDeviceLocal(ImageInfo& imageInfo)
	{
		if (imageInfo.mImageData != nullptr) {
			imageInfo.mUsageFlags = VK_IMAGE_USAGE_TRANSFER_DST_BIT | imageInfo.mUsageFlags;
		}

		imageInfo.mVmaMemoryUsage = VMA_MEMORY_USAGE_GPU_ONLY;
		std::shared_ptr<Image> image = std::make_shared<Image>(this, imageInfo);

		if (imageInfo.mImageData != nullptr) {
			BufferInfo bufferInfo{};
			bufferInfo.mDataSize = imageInfo.mImageSize;
			bufferInfo.mData = imageInfo.mImageData;
			bufferInfo.mUsageFlags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
			Buffer stagingBuffer(this, bufferInfo);

			ImageTransition copyTransition{};
			copyTransition.mFormat = imageInfo.mImageTransition.mFormat;
			copyTransition.mHasStencilComponent = imageInfo.mImageTransition.mHasStencilComponent;
			copyTransition.mOldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			copyTransition.mNewLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			transitionImageLayout(image->GetVkImage(), copyTransition);
			copyBufferToImage(stagingBuffer.GetVkBuffer(), image->GetVkImage(), imageInfo.mWidth, imageInfo.mHeight);
		}

		if (imageInfo.mImageTransition.mDoTransition) {
			transitionImageLayout(image->GetVkImage(), imageInfo.mImageTransition);
		}
		return image;
	}

	VkDevice VulkanMemoryManager::GetDevice()
	{
		return VulkanManager::GetInstance()->GetDevice();
	}

	void VulkanMemoryManager::AddBufferAllocation(Buffer* buffer, std::string bufferName)
	{
		m_bufferAllocations.insert({ buffer, bufferName });
	}

	void VulkanMemoryManager::RemoveBufferAllocation(Buffer* buffer)
	{
		m_bufferAllocations.erase(buffer);
	}

	void VulkanMemoryManager::AddImageAllocation(Image* image, std::string imageName)
	{
		m_imageAllocations.insert({ image, imageName });
	}

	void VulkanMemoryManager::RemoveImageAllocation(Image* image)
	{
		m_imageAllocations.erase(image);
	}

	void VulkanMemoryManager::CheckAllocations()
	{
		if (m_bufferAllocations.size() == 0 && m_imageAllocations.size() == 0)
		{
			return;
		}

		std::string allocations = "Some allocations have not been freed:\n";
		for (auto& [key, value] : m_bufferAllocations)
		{
			allocations += value;
			allocations += "\n";
		}

		for (auto& [key, value] : m_imageAllocations)
		{
			allocations += value;
			allocations += "\n";
		}
		err(allocations);
	}

	void VulkanMemoryManager::copyBuffer(Buffer& srcBuffer, Buffer& dstBuffer)
	{
		VkCommandBuffer commandBuffer = VulkanManager::GetInstance()->BeginSingleTimeCommands();

		VkBufferCopy copy{};
		copy.srcOffset = 0;
		copy.dstOffset = 0;
		copy.size = dstBuffer.GetSize();

		VulkanInterface::vkCmdCopyBuffer(commandBuffer, srcBuffer.GetVkBuffer() , dstBuffer.GetVkBuffer(), 1, &copy);

		VulkanManager::GetInstance()->EndSingleTimeCommands(commandBuffer);
	}

	void VulkanMemoryManager::transitionImageLayout(VkImage image, ImageTransition transition)
	{
		VkCommandBuffer commandBuffer = VulkanManager::GetInstance()->BeginSingleTimeCommands();

		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = transition.mOldLayout;
		barrier.newLayout = transition.mNewLayout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = image;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		if (transition.mNewLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

			if (transition.mHasStencilComponent) {
				barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
			}
		}
		else {
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		}
		
		VkPipelineStageFlags sourceStage;
		VkPipelineStageFlags destinationStage;

		if (transition.mOldLayout == VK_IMAGE_LAYOUT_UNDEFINED && transition.mNewLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (transition.mOldLayout == VK_IMAGE_LAYOUT_UNDEFINED && transition.mNewLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else if (transition.mOldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && transition.mNewLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else if (transition.mOldLayout == VK_IMAGE_LAYOUT_UNDEFINED && transition.mNewLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		}
		else {
			err("VulkanMemoryManager::transitionImageLayout: unsupported layout transition!");
		}


		VulkanInterface::vkCmdPipelineBarrier(
			commandBuffer,
			sourceStage,
			destinationStage,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		);

		VulkanManager::GetInstance()->EndSingleTimeCommands(commandBuffer);
	}

	void VulkanMemoryManager::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
	{
		VkCommandBuffer commandBuffer = VulkanManager::GetInstance()->BeginSingleTimeCommands();

		VkBufferImageCopy region{};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;

		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;

		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = {
			width,
			height,
			1
		};

		VulkanInterface::vkCmdCopyBufferToImage(
			commandBuffer,
			buffer,
			image,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1,
			&region
		);

		VulkanManager::GetInstance()->EndSingleTimeCommands(commandBuffer);
	}
}
