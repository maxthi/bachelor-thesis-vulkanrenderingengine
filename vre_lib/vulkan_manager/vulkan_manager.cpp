#include "vulkan_manager.hpp"
#include "../scene_manager/scene_manager.hpp"
#include "../core/vre_core.hpp"

namespace vre
{
	VulkanManager* VulkanManager::GetInstance()
	{
		static VulkanManager vulkanManager;
		return &vulkanManager;
	}

	void VulkanManager::AddSwapchainRecreationCallback(std::function<void(bool)> func)
	{
		m_swapchainRecreationCallbacks.push_back(func);
	}

	void VulkanManager::Create()
	{
		// get vulkan specific configuration
		m_vulkanConfiguration = &VreCore::GetInstance()->GetConfig()->mVulkanCfg;

		m_vulkanInstance = std::make_shared<VulkanInstance>();
		updateQVkInstDependencies();

		// core:
		m_physicalDevice = std::make_shared<PhysicalDevice>();
		m_logicalDevice = std::make_shared<LogicalDevice>();
		m_vulkanMemoryManager = std::make_shared<VulkanMemoryManager>();
		m_swapchain = std::make_shared<Swapchain>();
		m_commandPool = std::make_shared<CommandPool>();
		m_textureSampler = std::make_shared<TextureSampler>();
		
		// update swapchain size
		m_lastSwapchainSize = m_swapchain->GetSwapchainInfo()->size;
	}

	void VulkanManager::DestroyBeforeSurfaceDestroyed()
	{
		vkDeviceWaitIdle(m_logicalDevice->GetVkDevice());

		m_textureSampler.reset();
		m_commandPool.reset();
		m_swapchain.reset();
	}

	void VulkanManager::DestroyAfterSurfaceDestroyed()
	{
		m_vulkanMemoryManager.reset();
		m_logicalDevice.reset();
		m_vulkanInstance.reset();
	}

	void VulkanManager::RecreateSwapchain()
	{
		// wait for resources to be free
		VkDevice device = m_logicalDevice->GetVkDevice();
		VulkanInterface::vkDeviceWaitIdle(device);

		// destroy swapchain and recreate it
		m_swapchain.reset();
		m_swapchain = std::make_shared<Swapchain>();

		bool swapchainSizeChanged = m_swapchain->GetSwapchainInfo()->size != m_lastSwapchainSize;
		for (auto& callback : m_swapchainRecreationCallbacks)
		{
			callback(swapchainSizeChanged);
		}
		m_lastSwapchainSize = m_swapchain->GetSwapchainInfo()->size;
	}

	VkCommandPool VulkanManager::GetCommandPool()
	{
		checkPtr(m_commandPool, "VulkanManager::GetCommandPool: CommandPool is nullptr!");
		return m_commandPool->GetCommandPool();
	}

	void VulkanManager::SetQWindow(QWindow* window)
	{
		m_pQtWindow = window;
	}

    const QWindow* VulkanManager::GetQWindow()
	{
		//checkPtr(m_pQtWindow, "VulkanManager::GetQWindow: QWindow is nullptr!");
		return m_pQtWindow;
	}

    VkSurfaceKHR VulkanManager::GetSurfaceKHR()
	{
		if (m_surface == VK_NULL_HANDLE) {
			checkPtr(m_vulkanInstance, "VulkanManager::GetSurfaceKHR: VulkanInstance is nullptr!");
			m_surface =  m_vulkanInstance->GetQVulkanInstance().surfaceForWindow(m_pQtWindow);
		}
		return m_surface;
	}

	uint32_t VulkanManager::GetGraphicsQueueFamilyIndex()
	{
		checkPtr(m_physicalDevice, "VulkanManager::GetGraphicsQueueFamilyIndex: PhysicalDevice is nullptr!");
		return m_physicalDevice->GetQueueFamilyIndices()->graphicsFamily.value();
	}

	const SwapchainInfo* VulkanManager::GetSwapchainInfo()
	{
		return m_swapchain->GetSwapchainInfo();
	}

	VkSampler VulkanManager::GetTextureSampler()
	{
		checkPtr(m_textureSampler, "VulkanManager::GetTextureSampler: TextureSampler is nullptr!");
		return m_textureSampler->GetSampler();
	}

	VkCommandBuffer VulkanManager::BeginSingleTimeCommands()
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = GetCommandPool();
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		VulkanInterface::vkAllocateCommandBuffers(GetDevice(), &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		VulkanInterface::vkBeginCommandBuffer(commandBuffer, &beginInfo);

		return commandBuffer;
	}

	void VulkanManager::EndSingleTimeCommands(VkCommandBuffer commandBuffer)
	{
		VulkanInterface::vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		auto graphicsQueue = m_logicalDevice->GetVkQueueGraphics();
		VulkanInterface::vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
		VulkanInterface::vkQueueWaitIdle(graphicsQueue);

		VulkanInterface::vkFreeCommandBuffers(GetDevice(), GetCommandPool(), 1, &commandBuffer);
	}

	std::shared_ptr<Buffer> VulkanManager::CreateBufferMemoryMapped(BufferInfo& bufferInfo)
	{
		return m_vulkanMemoryManager->CreateBufferMemoryMapped(bufferInfo);
	}

	std::shared_ptr<Buffer> VulkanManager::CreateBufferDeviceLocal(BufferInfo& bufferInfo)
	{
		return m_vulkanMemoryManager->CreateBufferDeviceLocal(bufferInfo);
	}

	std::shared_ptr<Image> VulkanManager::CreateImageMemoryMapped(ImageInfo& imageInfo)
	{
		return m_vulkanMemoryManager->CreateImageMemoryMapped(imageInfo);
	}

	std::shared_ptr<Image> VulkanManager::CreateImageDeviceLocal(ImageInfo& imageInfo)
	{
		return m_vulkanMemoryManager->CreateImageDeviceLocal(imageInfo);
	}

	void VulkanManager::updateQVkInstDependencies()
	{
		QVulkanInstance* qVkInst = &(m_vulkanInstance->GetQVulkanInstance());
		VulkanInterface::VulkanInterface::Interface.SetQVulkanFunctions(qVkInst->functions());
		m_pQtWindow->setVulkanInstance(qVkInst);
	}
}
