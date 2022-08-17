#pragma once
#include "../common.hpp"

namespace vre
{

	/// @brief A texture sampler describes how a shader can sample from a vk image resource. 
	/// This is a default texture sampler to be used by the framework.
	class TextureSampler
	{
	public:
		TextureSampler();
		~TextureSampler();

		/// @brief Get the vk habndle of the texture sampler.
		VkSampler GetSampler();

	private:
		/// @brief vk handle of the sampler.
		VkSampler m_sampler{ VK_NULL_HANDLE };
	};
}
