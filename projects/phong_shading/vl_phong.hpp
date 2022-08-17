#pragma once
#include <shader_manager/vertex_layout.hpp>

namespace vre
{
	class Drawable;

	class VertexLayoutPhong : public VertexLayout
	{
		/// @brief This strucutre describes the vertex layout and is only used to calculate the offsets with
		// offsetof(Vertex, <attr>);
		struct Vertex
		{
			float position[3];
			float texCoords[2];
			float normal[3];
		};

	public:
		VertexLayoutPhong();
		~VertexLayoutPhong() = default;
		std::shared_ptr<VertexAndIndexBuffer> CreateVertexAndIndexBuffer(std::shared_ptr<Drawable> drawable) override;
	};
}