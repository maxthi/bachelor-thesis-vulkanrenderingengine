#pragma once
#include "../../common.hpp"
#include "../../shader_manager/vertex_layout.hpp"
#include "../../scene_helper/utility/vertex_buffer_creator.hpp"

namespace vre
{
	class Drawable;

	/// @brief Defines the layout of the data passed to the vertex shader for textured objects.
	class VertexLayoutTexture : public VertexLayout
	{
		/// @brief A class local definition of the structure per vertex
		/// 3 * 4 bytes per vertex position.
		/// 2 * 4 bytes per vertex texture coord.
		struct Vertex
		{
			float position[3];
			float texCoords[2];
		};

	public:
		VertexLayoutTexture();
		~VertexLayoutTexture() = default;
		std::shared_ptr<VertexAndIndexBuffer> CreateVertexAndIndexBuffer(std::shared_ptr<Drawable> drawable) override;
	};
}