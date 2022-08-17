#pragma once
#include <QWindow>
#include "../common.hpp"

// vulkan memory management
#include "vulkan_memory_manager.hpp"

// renderer connection
#include "../vulkan_renderer/renderer.hpp"

// vulkan manager objects
#include "vulkan_instance.hpp"
#include "physical_device.hpp"
#include "logical_device.hpp"
#include "swapchain.hpp"
#include "vulkan_configuration.hpp"
#include "texture_sampler.hpp"
#include "command_pool.hpp"


namespace vre
{

	/// @brief The VulkanManager builds the interface for primary basic vulkan objects.
	class VulkanManager
	{
	public:
		static VulkanManager* GetInstance();

		void Create();
	
		void DestroyBeforeSurfaceDestroyed();
		void DestroyAfterSurfaceDestroyed();

		/// @brief Called if the swapchain has to be recreated. Afterwards all registered swapchain recreation callbacks
		/// are notified.
		void RecreateSwapchain();

		/// @brief Set the qt window handle for the vulkan manager.
		void SetQWindow(QWindow* qtWindow);

		/// @brief Get the qt window handle.
		const QWindow* GetQWindow();

		// from VulkanInstance
		std::shared_ptr<VulkanInstance> GetVulkanInstance() { return m_vulkanInstance; }

		// physical device access
		std::shared_ptr<PhysicalDevice> GetPhysicalDevice() { return m_physicalDevice; }
		uint32_t GetGraphicsQueueFamilyIndex();

		// from LogicalDevice
		std::shared_ptr<LogicalDevice> GetLogicalDevice() { return m_logicalDevice; }
		VkDevice GetDevice() { return m_logicalDevice->GetVkDevice(); }

		// from Swapchain
		const SwapchainInfo* GetSwapchainInfo();
		VkCommandPool GetCommandPool();
		VkSampler GetTextureSampler();

		std::shared_ptr<VulkanMemoryManager> GetMemoryManager() { return m_vulkanMemoryManager; }


		// vulkan manager functionality
		VkSurfaceKHR GetSurfaceKHR();

		VkCommandBuffer BeginSingleTimeCommands();
		void EndSingleTimeCommands(VkCommandBuffer commandBuffer);

		// CPU mappable buffer
		std::shared_ptr<Buffer> CreateBufferMemoryMapped(BufferInfo& bufferInfo);

		// uses staging buffer to transfer data into device memory
		std::shared_ptr<Buffer> CreateBufferDeviceLocal(BufferInfo& bufferInfo);

		/// @brief Creates image memory on the GPU that is mapped to the CPU and can be written to directly.
		/// @param imageInfo Details about image creation.
		/// @return A shared pointer to the image.
		std::shared_ptr<Image> CreateImageMemoryMapped(ImageInfo& imageInfo);

		/// @brief Creates image memory on the GPU in the fast device local memory. Cannot be written directly, but with a staging buffer memory can be transfered.
		/// @param imageInfo  Details about the image creation.
		/// @return A shared pointer to the image.
		std::shared_ptr<Image> CreateImageDeviceLocal(ImageInfo& imageInfo);

		/// @brief Returns the vulkan configuration.
		VulkanConfiguration* GetVulkanConfiguration() { return m_vulkanConfiguration; }

		void AddSwapchainRecreationCallback(std::function<void(bool)> func);

	private:
		VulkanManager() = default;

		QWindow* m_pQtWindow{};

		std::shared_ptr<VulkanInstance> m_vulkanInstance;
		std::shared_ptr<PhysicalDevice>	m_physicalDevice;
		std::shared_ptr<LogicalDevice> m_logicalDevice;
		std::shared_ptr<Swapchain> m_swapchain;
		std::shared_ptr<CommandPool> m_commandPool;
		std::shared_ptr<VulkanMemoryManager> m_vulkanMemoryManager;

		/// @brief A default texture sampler, used to sample textures in shaders.
		std::shared_ptr<TextureSampler> m_textureSampler;

		/// @brief A pointer to the Qt surface provided by Qt.
		VkSurfaceKHR m_surface{};

		/// @brief Saving the number of swapchain images. In case of swapchain recreation, its tested whether the
		/// number of swapchain images has changed.
		size_t m_lastSwapchainSize{};

		VulkanConfiguration* m_vulkanConfiguration;

		void updateQVkInstDependencies();

		std::list<std::function<void(bool)>> m_swapchainRecreationCallbacks;
	};
}
