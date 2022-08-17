#pragma once
#include "../../common.hpp"
#include "../../shader_manager/vertex_layout.hpp"

namespace vre
{
	class Drawable;

	/// @brief Defines the layout of the data passed to the vertex shader for colored objects.
	class VertexLayoutColor : public VertexLayout
	{
		/// @brief A class local definition of the structure per vertex
		/// 3 * 4 bytes per vertex position.
		/// 4 * 4 bytes per vertex color.
		struct Vertex
		{
			float position[3];
			float color[4];
		};

	public:
		VertexLayoutColor();
		~VertexLayoutColor() = default;
		std::shared_ptr<VertexAndIndexBuffer> CreateVertexAndIndexBuffer(std::shared_ptr<Drawable> drawable) override;
	};
}