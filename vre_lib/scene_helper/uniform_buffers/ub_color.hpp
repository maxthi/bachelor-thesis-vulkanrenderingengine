#pragma once
#include "../../common.hpp"
#include "../../shader_manager/uniform_buffer.hpp"
#include "../../vulkan_manager/buffer.hpp"
#include "../descriptor_set_layouts/dsl_color.hpp"
#include "../../scene_reader/structs/drawable_data.hpp"

namespace vre
{
	/// @brief Class to create uniform buffer objects that provide access to the drawables attached data
	/// and to the mvp matrix.
	class UniformBufferColor : public UniformBuffer
	{
	public:
		/// @brief The DescriptorLayout defines how the uniform buffer is structured.
		static std::shared_ptr<DescriptorSetLayout> GetDescriptorSetLayout();

		/// @brief Create memory on the gpu for the passed drawable.
		/// @param drawableData - The drawable to create the uniform buffer for.
		void Create(std::shared_ptr<DrawableData> drawableData);
	};
}
