#include "logical_device.hpp"
#include "vulkan_manager.hpp"

namespace vre
{
	LogicalDevice::LogicalDevice()
	{
		//uint32_t graphicsFamilyIndex = m_pVulkanManager->GetGraphicsFamilyIndex();
		auto indices = VulkanManager::GetInstance()->GetPhysicalDevice()->GetQueueFamilyIndices();
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { indices->graphicsFamily.value(), indices->presentFamily.value() };

		float queuePriority = 1.0f;
		for (uint32_t queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures{};
		deviceFeatures.samplerAnisotropy = VK_TRUE;

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.pEnabledFeatures = &deviceFeatures;

		const std::vector<const char*>* deviceExtensions = VulkanManager::GetInstance()->GetPhysicalDevice()->GetExtensions();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions->size());
		createInfo.ppEnabledExtensionNames = deviceExtensions->data();
		createInfo.enabledLayerCount = 0;

		VkPhysicalDevice device = VulkanManager::GetInstance()->GetPhysicalDevice()->GetVkPhysicalDevice();
		checkVk(
			VulkanInterface::vkCreateDevice(device, &createInfo, nullptr, &m_vkDevice),
			"LogicalDevice::createLogicalDevice: Failed to create logical device!"
		);

		// aquire device functions
		VulkanInterface::VulkanInterface::Interface.SetQVulkanDeviceFunctions(VulkanManager::GetInstance()->GetVulkanInstance()->GetQVulkanInstance().deviceFunctions(m_vkDevice));

		VulkanInterface::vkGetDeviceQueue(m_vkDevice, indices->graphicsFamily.value(), 0, &m_vkQueueGraphics);
		VulkanInterface::vkGetDeviceQueue(m_vkDevice, indices->presentFamily.value(), 0, &m_vkQueuePresent);
	}

	LogicalDevice::~LogicalDevice()
	{
		VulkanInterface::vkDestroyDevice(m_vkDevice, nullptr);
	}

	VkDevice LogicalDevice::GetVkDevice()
	{
		checkPtr(m_vkDevice, "LogicalDevice::GetVkDevice: Device not created!");
		return m_vkDevice;
	}

	VkQueue LogicalDevice::GetVkQueueGraphics()
	{
		checkPtr(m_vkQueueGraphics, "LogicalDevice::GetVkQueueGraphics: GraphicsQueue not created!");
		return m_vkQueueGraphics;
	}

    VkQueue LogicalDevice::GetVkQueuePresent()
	{
		checkPtr(m_vkQueuePresent, "LogicalDevice::GetVkQueuePresent: PresentQueue not created!");
		return m_vkQueuePresent;
	}

}
