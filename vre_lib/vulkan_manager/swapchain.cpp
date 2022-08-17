#include "swapchain.hpp"
#include "vulkan_manager.hpp"

namespace vre
{
    Swapchain::Swapchain()
    {
        m_swapchainSupportDetails = *VulkanManager::GetInstance()->GetPhysicalDevice()->GetSwapchainSupportDetails();

        VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat();
        VkPresentModeKHR presentMode = chooseSwapPresentMode();
        VkExtent2D extent = chooseSwapExtent(VulkanManager::GetInstance()->GetQWindow()->width(), VulkanManager::GetInstance()->GetQWindow()->height());

        m_imageFormat = surfaceFormat.format;
        m_extent = extent;

        // images in swap chain
        uint32_t imageCount = m_swapchainSupportDetails.capabilities.minImageCount + 1;
        // don't exceed maximum (0 => no maxium)
        if (m_swapchainSupportDetails.capabilities.maxImageCount > 0 && imageCount > m_swapchainSupportDetails.capabilities.maxImageCount) {
            imageCount = m_swapchainSupportDetails.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = VulkanManager::GetInstance()->GetSurfaceKHR();
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        const QueueFamilyIndices indices = *VulkanManager::GetInstance()->GetPhysicalDevice()->GetQueueFamilyIndices();
        uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

        if (indices.graphicsFamily != indices.presentFamily) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0; // Optional
            createInfo.pQueueFamilyIndices = nullptr; // Optional
        }

        createInfo.preTransform = m_swapchainSupportDetails.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        VkDevice device = VulkanManager::GetInstance()->GetDevice();
        checkVk(
            VulkanInterface::vkCreateSwapchainKHR(device, &createInfo, nullptr, &m_swapchain),
            "Swapchain::Create: Failed to create swap chain!"
        );

        // the real size of the swapchain has to be queried after the creation, since only a minimum was given
        // to create the swapchain
        VulkanInterface::vkGetSwapchainImagesKHR(device, m_swapchain, &imageCount, nullptr);
        m_images.resize(imageCount);
        VulkanInterface::vkGetSwapchainImagesKHR(device, m_swapchain, &imageCount, m_images.data());

        createImageViews();

        // initialize or update info structure with new swapchain details
        m_swapchainInfo.swapchain = m_swapchain;
        m_swapchainInfo.size = imageCount;
        m_swapchainInfo.format = surfaceFormat.format;
        m_swapchainInfo.extent2D = extent;
        m_swapchainInfo.pImageViews = &m_imageViews;
    }

    Swapchain::~Swapchain()
    {
        VkDevice device = VulkanManager::GetInstance()->GetDevice();
        for (auto imageView : m_imageViews) {
            VulkanInterface::vkDestroyImageView(device, imageView, nullptr);
        }
        VulkanInterface::vkDestroySwapchainKHR(device, m_swapchain, nullptr);
    }

    const SwapchainInfo* Swapchain::GetSwapchainInfo()
    {
        checkPtr(m_swapchain, "Swapchain::GetSwapchainSize: Swapchain not created!");
        return &m_swapchainInfo;
    }

    VkSurfaceFormatKHR Swapchain::chooseSwapSurfaceFormat()
    {
        for (const auto& availableFormat : m_swapchainSupportDetails.formats) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return availableFormat;
            }
        }

        return m_swapchainSupportDetails.formats[0];
    }

    VkPresentModeKHR Swapchain::chooseSwapPresentMode()
    {
        for (const auto& availablePresentMode : m_swapchainSupportDetails.presentModes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return availablePresentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D Swapchain::chooseSwapExtent(uint32_t windowWidth, uint32_t windowHeight)
    {
        const VkSurfaceCapabilitiesKHR& capabilities = m_swapchainSupportDetails.capabilities;
        if (capabilities.currentExtent.width != UINT32_MAX) {
            return capabilities.currentExtent;
        }
        else {
            VkExtent2D actualExtent = {windowWidth, windowHeight};

            actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
            actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

            return actualExtent;
        }
    }

    void Swapchain::createImageViews()
    {
        m_imageViews.resize(m_images.size());
        for (size_t i = 0; i < m_images.size(); i++) {
            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = m_images[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = m_imageFormat;
            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            checkVk(
                VulkanInterface::vkCreateImageView(VulkanManager::GetInstance()->GetDevice(), &createInfo, nullptr, &m_imageViews[i]),
                "Swapchain::createImageViews: failed to create image view!"
            );
        }
    }
}
