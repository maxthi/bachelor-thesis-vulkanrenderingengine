#include "command_pool.hpp"
#include "vulkan_manager.hpp"

namespace vre {
    CommandPool::CommandPool()
    {
        // create command pool
        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = VulkanManager::GetInstance()->GetGraphicsQueueFamilyIndex();

        VkDevice device = VulkanManager::GetInstance()->GetDevice();
        checkVk(
            VulkanInterface::vkCreateCommandPool(device, &poolInfo, nullptr, &m_commandPool),
            "Commandbuffers::Create: failed to create command pool!"
        );
    }

    CommandPool::~CommandPool()
    {
        if (m_commandPool == VK_NULL_HANDLE)
        {
            return;
        }
        VkDevice device = VulkanManager::GetInstance()->GetDevice();
        VulkanInterface::vkDestroyCommandPool(device, m_commandPool, nullptr);
        m_commandPool = VK_NULL_HANDLE;
    }


    VkCommandPool CommandPool::GetCommandPool()
    {
        checkPtr(m_commandPool, "CommandPool::GetCommandPool: CommandPool not created!");
        return m_commandPool;
    }
}
