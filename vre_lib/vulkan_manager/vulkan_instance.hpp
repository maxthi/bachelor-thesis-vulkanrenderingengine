#pragma once
#include "../common.hpp"

namespace vre
{
	/// @brief A wrapper for the vulkan instance provided by Qt.
	class VulkanInstance
	{
	public:
		VulkanInstance();
		~VulkanInstance();

		/// @brief Get the qt vulkan instance.
		QVulkanInstance& GetQVulkanInstance();

        /// @brief Get the vulkan instance.
        VkInstance GetVulkanInstance();

	private:
		/// @brief The qt vulkan instance.
		QVulkanInstance m_qVkInst;

		/// @brief Check if the required valdiation layers are supported by the instance.
		void checkValidationLayerSupport();
	};
}
