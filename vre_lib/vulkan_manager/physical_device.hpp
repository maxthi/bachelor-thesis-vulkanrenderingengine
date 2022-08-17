#pragma once
#include "../common.hpp"

// vulkan manager objects
#include "vulkan_instance.hpp"

namespace vre
{
	class PhysicalDevice
	{
	public:
		PhysicalDevice();

        /// @brief Get the handle to the physical device.
        VkPhysicalDevice GetVkPhysicalDevice();

        /// @brief The queue family indices of the physical device.
        const QueueFamilyIndices* GetQueueFamilyIndices();

        /// @brief The available physical device extensions.
        const std::vector<const char*>* GetExtensions();

        /// @brief Get swapchain support details.
        const SwapchainSupportDetails* GetSwapchainSupportDetails();

		/// @brief Sample count flag bits.
		VkSampleCountFlagBits GetSampleCountFlagBits();

		/// @brief Returns one of the given format candidates if the pyhsical device supports the desired tiling and features.
		/// @param candidates - The format candidates to test.
		/// @param tiling - The desired tiling.
		/// @param features - The desired features.
		/// @return The format supporting both the tiling and features.
		VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

	private:
		/// @brief Handle to the physical device.
		VkPhysicalDevice m_physicalDevice{};

		/// @brief Device extensions that should be loaded.
		const std::vector<const char*> m_deviceExtensions{ VK_KHR_SWAPCHAIN_EXTENSION_NAME };

		/// @brief The identified swapchain support.
		SwapchainSupportDetails m_swapchainSupportDetails{};

		/// @brief The identified queue family indices.
		std::optional<QueueFamilyIndices> m_queueFamilyIndices;

		/// @brief The supported sample count for multisampling.
		VkSampleCountFlagBits m_msaaSamplesCountAvailable{ VK_SAMPLE_COUNT_1_BIT };

		/// @brief Rates the suitability of a given physical device by certain properties.
		/// @param device - The device to rate.
		/// @return - The rating score.
		int rateDeviceSuitability(VkPhysicalDevice device);

		/// @brief Find the queue family indices for a physical device.
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

		/// @brief Checks if the extensions added to m_device extensions are supported by the device.
		/// @param device - The physical device.
		/// @return True if all extensions are supported.
		bool checkDeviceExtensionSupport(VkPhysicalDevice device);

		/// @brief Query swapchain support.
		/// @param device - The device to check the swapchain support for.
		/// @return - The struct containing the information.
		SwapchainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

		/// @brief Query multisampling support.
		void queryMSAASampleCount();
	};
}
