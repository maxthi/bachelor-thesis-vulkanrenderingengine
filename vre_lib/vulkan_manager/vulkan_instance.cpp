#include "vulkan_instance.hpp"
#include "vulkan_manager.hpp"

namespace vre
{
	VulkanInstance::VulkanInstance()
	{
		auto config = VulkanManager::GetInstance()->GetVulkanConfiguration();
		if (config->mEnableValidationLayers) {
			checkValidationLayerSupport();

			QByteArrayList list = QByteArrayList();
			for (const char* layer : config->mValidationLayers) {
				list << layer;
			}
			m_qVkInst.setLayers(list);
		}

		if (!m_qVkInst.create()) {
			err("VulkanInstance::VulkanInstance: Failed to create QVulkanInstance!");
		}
	}

	VulkanInstance::~VulkanInstance()
	{
		if (m_qVkInst.isValid()) {
			m_qVkInst.destroy();
		}
	}

	QVulkanInstance& VulkanInstance::GetQVulkanInstance()
	{
		return m_qVkInst;
	}

    VkInstance VulkanInstance::GetVulkanInstance()
	{
		return m_qVkInst.vkInstance();
	}

	void VulkanInstance::checkValidationLayerSupport()
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		auto config = VulkanManager::GetInstance()->GetVulkanConfiguration();

		for (const char* layerName : config->mValidationLayers) {
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers) {
				if (strcmp(layerName, layerProperties.layerName) == 0) {
					layerFound = true;
					break;
				}
			}

			if (!layerFound) {
				err("VulkanInstance::checkValiatationLayerSupport: Missing validation layer: " + std::string(layerName));
			}
		}
	}
};
