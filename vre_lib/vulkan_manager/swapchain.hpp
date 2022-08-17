#pragma once
#include "../common.hpp"

// vulkan manager objects
#include "physical_device.hpp"
#include "logical_device.hpp"

namespace vre
{
	/// @brief Details of a swapchain.
	struct SwapchainInfo
	{
		/// @brief The vk handle to the swapchain.
		VkSwapchainKHR swapchain;

		/// @brief Swapchain size. In other words the number of images that the swapchain manages.
		size_t size;

		/// @brief Format of the swapchain images.
		VkFormat format;

		/// @brief Dimensions of the images.
		VkExtent2D extent2D;

		/// @brief Handles to the image views of the swapchain images.
		const std::vector<VkImageView>* pImageViews;
	};

	/// @brief A wrapper class responsible for creating the vk swapchain and determining properties of it.
	class Swapchain
	{
	public:
		Swapchain();
		~Swapchain();

		/// @brief Get a pointer to swapchain info.
		const SwapchainInfo* GetSwapchainInfo();

	private:
		/// @brief The swapchain info of the created swapchain.
		SwapchainInfo m_swapchainInfo;

		/// @brief The vk handle of the swapchain.
		VkSwapchainKHR m_swapchain{ VK_NULL_HANDLE };

		/// @brief Swapchain support details retrieved via pyhsical device.
		SwapchainSupportDetails m_swapchainSupportDetails;

		/// @brief The images of the swapchain.
		std::vector<VkImage> m_images;

		/// @brief The image views of the images.
		std::vector<VkImageView> m_imageViews;

		/// @brief The image format.
		VkFormat m_imageFormat{};

		/// @brief The image dimensions.
		VkExtent2D m_extent{};

		/// @brief Choses a format based on hardcoded values.
		VkSurfaceFormatKHR chooseSwapSurfaceFormat();

		/// @brief Method choses the present mode.
		VkPresentModeKHR chooseSwapPresentMode();

		/// @brief Choses the extent of the swapchain, in the range of the supported values.
		VkExtent2D chooseSwapExtent(uint32_t windowWidth, uint32_t windowHeight);

		/// @brief Create image views for the swap chain images.
		void createImageViews();
	};
}
