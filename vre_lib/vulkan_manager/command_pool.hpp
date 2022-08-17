#pragma once
#include "../common.hpp"

namespace vre
{
	/// @brief Wraps a vulkan command pool.
	class CommandPool
	{
	public:
		/// @brief Default constructor.
		CommandPool();

		/// @brief Deconstructor deletes command pool.
		~CommandPool();

		/// @brief Returns the vk handle to the command pool.
		VkCommandPool GetCommandPool();

	private:
		VkCommandPool m_commandPool{ VK_NULL_HANDLE };
	};
}
