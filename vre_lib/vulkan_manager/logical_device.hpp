#pragma once
#include "../common.hpp"

// vulkan manager objects
#include "physical_device.hpp"

namespace vre 
{
	class VulkanManager;

	/// @brief Warpper class for vulkan logical devices.
	class LogicalDevice
	{
	public:
		LogicalDevice();
		~LogicalDevice();

        VkDevice GetVkDevice();
        VkQueue GetVkQueueGraphics();
        VkQueue GetVkQueuePresent();

	private:
		/// @brief Handle to the vulkan device.
		VkDevice m_vkDevice{};

		/// queues are implicitly cleaned up by vulkan when logical device gets destroyed
		VkQueue m_vkQueueGraphics{};
		VkQueue m_vkQueuePresent{};
	};
}
