#pragma once
#include "vertex_and_index_buffer.hpp"
#include "../../vulkan_manager/buffer.hpp"
#include "../../vulkan_manager/image.hpp"
#include "../../scene_manager/scene_objects/node.hpp"

namespace vre
{
	class UniformBuffer;
	class Drawable;
	/// @brief Used to store data along with a drawable.
	struct DrawableData
	{
		/// @brief A unique Id
		uint64_t mUniqueId{ s_uniqueId++ };

		/// @brief The node, the drawable is attached too.
		/// The node is required to calculate the transformation.
		std::shared_ptr<Node> mNode;

		/// @brief The drawable that these data is created from.
		std::shared_ptr<Drawable> mDrawable;

		/// @brief Contains the transformation of this drawable.
		std::shared_ptr<TransformationNode> mTransformationNode;

		/// @brief A data buffer with the vertices of the object in the first part and the indices afterwards.
		std::shared_ptr<VertexAndIndexBuffer> mVertexAndIndexBuffer;

		/// @brief The uniform buffers linking the vulkan buffers and the desciptor sets.
		/// Needs to exist per drawable-data instead of per drawable, because every reused drawable (see render targets)
		/// needs its own uniform buffer set.
		std::shared_ptr<UniformBuffer> mUniformBuffer;

		/// @brief The transformation for the drawable.
		std::shared_ptr<QMatrix4x4> mTransformation;

	private:
		static inline uint64_t s_uniqueId{ 0 };

	};
}