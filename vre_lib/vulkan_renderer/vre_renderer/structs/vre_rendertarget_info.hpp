#pragma once
#include "../../../vulkan_manager/vulkan_manager.hpp"

namespace vre::Renderer::VreRenderer
{
	struct VreRenderTargetInfo
	{
		uint32_t mHeight;
		uint32_t mWidth;
		VkFormat mFormat;
		const std::vector<VkImageView>* mImageViews;

		/// @brief The clear values for the attached color buffer.
		float mClearValuesColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

		/// @brief The clear values for the attached depth buffer.
		float mClearValuesDepth = 1.0f;
		
		uint32_t mClearValuesStencil = 0;
	};
}
