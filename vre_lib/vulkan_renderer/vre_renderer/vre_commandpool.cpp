#include "vre_commandpool.hpp"

namespace vre::Renderer::VreRenderer
{
    void VreCommandPool::Create()
    {
        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = VulkanManager::GetInstance()->GetGraphicsQueueFamilyIndex();
        // see https://vulkan-tutorial.com/Drawing_a_triangle/Drawing/Command_buffers
        poolInfo.flags =
            VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | // Hint that command buffers are rerecorded with new commands very often (may change memory allocation behavior)
            VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; // Allow command buffers to be rerecorded individually, without this flag they all have to be reset together

        VkDevice device = VulkanManager::GetInstance()->GetDevice();
        checkVk(
            VulkanInterface::vkCreateCommandPool(device, &poolInfo, nullptr, &m_commandPool),
            "Commandbuffers::Create: failed to create command pool!"
        );
    }

    void VreCommandPool::Destroy()
    {
        VkDevice device = VulkanManager::GetInstance()->GetDevice();
        VulkanInterface::vkDestroyCommandPool(device, m_commandPool, nullptr);
        m_commandPool = nullptr;
    }
}