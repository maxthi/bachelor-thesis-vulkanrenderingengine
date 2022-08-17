#pragma once
#include "../../../common.hpp"
#include "../../../vulkan_manager/buffer.hpp"
#include "../../../shader_manager/uniform_buffer.hpp"
#include "../../../shader_manager/shader_combination.hpp"
#include "../../../scene_manager/scene.hpp"
#include "vre_pipelineset.hpp"

namespace vre::Renderer::VreRenderer
{
	/// @brief Groups drawables that can be rendered with the same shader combination / pipeline.
	struct DrawableList
	{
		std::shared_ptr<ShaderCombination> mShaderCombination;
		std::vector<std::shared_ptr<DrawableData>> mDrawableDataList;

		/// @brief The according pipelineset.
		std::weak_ptr<VrePipelineSet> mPipelineSet;
	};

}
