#include "physical_device.hpp"
#include "vulkan_manager.hpp"

namespace vre
{
	PhysicalDevice::PhysicalDevice()
	{
		VkInstance vkInst = VulkanManager::GetInstance()->GetVulkanInstance()->GetVulkanInstance();
		uint32_t deviceCount = 0;
		VulkanInterface::vkEnumeratePhysicalDevices(vkInst, &deviceCount, nullptr);

		if (deviceCount == 0) {
			err("PhysicalDevice::pickPhysicalDevice: Failed to find a physical device with Vulkan support!");
		}

		std::vector<VkPhysicalDevice> devices(deviceCount);
		VulkanInterface::vkEnumeratePhysicalDevices(vkInst, &deviceCount, devices.data());

		std::multimap<int, VkPhysicalDevice> candidates;

		for (const auto& device : devices) {
			int score = rateDeviceSuitability(device);
			candidates.insert(std::make_pair(score, device));
		}

		for (auto it = candidates.rbegin(); it != candidates.rend(); ++it) {
			if (it->first > 0) {
				m_physicalDevice = it->second;
				break;
			}
		}

		if (m_physicalDevice == nullptr) {
			err("PhysicalDevice::pickPhysicalDevice: Failed to find a suitabale physical device!");
		}
		queryMSAASampleCount();
		m_swapchainSupportDetails = querySwapChainSupport(m_physicalDevice);
	}

	VkPhysicalDevice PhysicalDevice::GetVkPhysicalDevice()
	{
		checkPtr(m_physicalDevice, "PhysicalDevice::GetPhysicalDevice: PhysicalDevice not created!");
		return m_physicalDevice;
	}

    const QueueFamilyIndices* PhysicalDevice::GetQueueFamilyIndices()
	{
		if (m_physicalDevice == nullptr) {
			err("PhysicalDevice::GetQueueFamiliyIndices: Physical device is null!");
		}

		if (!m_queueFamilyIndices.has_value())
		{
			m_queueFamilyIndices = findQueueFamilies(m_physicalDevice);
		}

		return &(m_queueFamilyIndices.value());
	}

	const std::vector<const char*>* PhysicalDevice::GetExtensions()
	{
		return &m_deviceExtensions;
	}

	const SwapchainSupportDetails* PhysicalDevice::GetSwapchainSupportDetails()
	{
		m_swapchainSupportDetails = querySwapChainSupport(m_physicalDevice);
		return &m_swapchainSupportDetails;
	}

	VkSampleCountFlagBits PhysicalDevice::GetSampleCountFlagBits()
	{
		return m_msaaSamplesCountAvailable;
	}

	VkFormat PhysicalDevice::FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
	{
		for (VkFormat format : candidates)
		{
			VkFormatProperties props;
			VulkanInterface::vkGetPhysicalDeviceFormatProperties(m_physicalDevice, format, &props);
			if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
				return format;
			}
			else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
				return format;
			}
		}

		err("PhysicalDevice::FindSupportedFormat: None of the given formats is supported!");
		return VK_FORMAT_MAX_ENUM;
	}

	int PhysicalDevice::rateDeviceSuitability(VkPhysicalDevice device)
	{
		int rating = 0;

		VkPhysicalDeviceProperties deviceProperties;
		VulkanInterface::vkGetPhysicalDeviceProperties(device, &deviceProperties);

		VkPhysicalDeviceFeatures deviceFeatures;
		VulkanInterface::vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
		if (!deviceFeatures.samplerAnisotropy) {
			return 0;
		}

		// discard devices that don't support all required queue families
		QueueFamilyIndices indices = findQueueFamilies(device);
		if (!indices.isComplete())
			return 0;

		// discard devices that don't support the specified extensions
		if (!checkDeviceExtensionSupport(device))
			return 0;

		// discard devices that do not support a single format or not a single present mode
		SwapchainSupportDetails swapChainSupport = querySwapChainSupport(device);
		if (swapChainSupport.formats.empty() || swapChainSupport.presentModes.empty())
			return 0;

		// discrete GPUs have a significant performance advantage
		if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
			rating += 10000;
		}

		return rating;
	}

	QueueFamilyIndices PhysicalDevice::findQueueFamilies(VkPhysicalDevice device)
	{
		QueueFamilyIndices indices;
		uint32_t queueFamilyCount = 0;
		VulkanInterface::vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		VulkanInterface::vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		VkBool32 presentSupport = false;
		VkSurfaceKHR surface = VulkanManager::GetInstance()->GetSurfaceKHR();
		for (const auto& queueFamily : queueFamilies) {
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.graphicsFamily = i;
			}

			presentSupport = false;
			VulkanInterface::vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

			if (presentSupport) {
				indices.presentFamily = i;
			}

			if (indices.isComplete()) {
				break;
			}

			i++;
		}
		return indices;
	}

	bool PhysicalDevice::checkDeviceExtensionSupport(VkPhysicalDevice device) {
		uint32_t extensionCount;
		VulkanInterface::vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		VulkanInterface::vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(m_deviceExtensions.begin(), m_deviceExtensions.end());

		for (const auto& extension : availableExtensions) {
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}

	SwapchainSupportDetails PhysicalDevice::querySwapChainSupport(VkPhysicalDevice device) {
		SwapchainSupportDetails details;

		/*
		There are basically three kinds of properties we need to check:
			Basic surface capabilities(min / max number of images in swap chain, min / max width and height of images)
			Surface formats(pixel format, color space)
			Available presentation modes
		*/

		VkSurfaceKHR surface = VulkanManager::GetInstance()->GetSurfaceKHR();

		// query capabilities
		VulkanInterface::vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

		// query formats
		uint32_t formatCount;
		VulkanInterface::vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

		if (formatCount != 0) {
			details.formats.resize(formatCount);
			VulkanInterface::vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
		}

		// query presentation mode
		uint32_t presentModeCount;
		VulkanInterface::vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

		if (presentModeCount != 0) {
			details.presentModes.resize(presentModeCount);
			VulkanInterface::vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
		}

		return details;
	}

	void PhysicalDevice::queryMSAASampleCount()
	{
		VkPhysicalDeviceProperties deviceProperties;
		VulkanInterface::vkGetPhysicalDeviceProperties(m_physicalDevice, &deviceProperties);

		// check available color and depthdepth buffer sample counts
		VkSampleCountFlags counts = deviceProperties.limits.framebufferColorSampleCounts & deviceProperties.limits.framebufferDepthSampleCounts;
		if (counts & VK_SAMPLE_COUNT_64_BIT) { m_msaaSamplesCountAvailable = VK_SAMPLE_COUNT_64_BIT; return; }
		if (counts & VK_SAMPLE_COUNT_32_BIT) { m_msaaSamplesCountAvailable = VK_SAMPLE_COUNT_32_BIT; return; }
		if (counts & VK_SAMPLE_COUNT_16_BIT) { m_msaaSamplesCountAvailable = VK_SAMPLE_COUNT_16_BIT; return;}
		if (counts & VK_SAMPLE_COUNT_8_BIT) { m_msaaSamplesCountAvailable = VK_SAMPLE_COUNT_8_BIT; return; }
		if (counts & VK_SAMPLE_COUNT_4_BIT) { m_msaaSamplesCountAvailable = VK_SAMPLE_COUNT_4_BIT; return; }
		if (counts & VK_SAMPLE_COUNT_2_BIT) { m_msaaSamplesCountAvailable = VK_SAMPLE_COUNT_2_BIT; return; }
	}
}
