#include "vl_color.hpp"
#include "../../scene_manager/scene_objects/drawable.hpp"
#include "../../scene_manager/drawable_properties/mesh.hpp"
#include "../../scene_helper/utility/vertex_buffer_creator.hpp"

namespace vre
{
	VertexLayoutColor::VertexLayoutColor()
	{
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		m_bindingDescriptions.push_back(bindingDescription);

		VkVertexInputAttributeDescription attributeDescription{};
		attributeDescription.binding = 0;
		attributeDescription.location = 0;
		attributeDescription.format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescription.offset = offsetof(Vertex, position);
		m_attributeDescriptions.push_back(attributeDescription);

		attributeDescription.binding = 0;
		attributeDescription.location = 1;
		attributeDescription.format = VK_FORMAT_R32G32B32A32_SFLOAT;
		attributeDescription.offset = offsetof(Vertex, color);
		m_attributeDescriptions.push_back(attributeDescription);
	}

	std::shared_ptr<VertexAndIndexBuffer> VertexLayoutColor::CreateVertexAndIndexBuffer(std::shared_ptr<Drawable> drawable)
	{	
		// arranging vertex positions and tex coords together in a uint32_t buffer
		std::shared_ptr<Mesh> mesh = drawable->GetProperty<Mesh>();
		auto vertices = mesh->GetVertices();
		auto colors = mesh->GetColors();

		// 3 floats per position, 4 floats per color
		uint8_t stride = 7;
		VertexBufferCreator<float> vertexBufferCreator(mesh, stride);

		// if only one color is given, use this color for all vertices
		auto color = colors->at(0);
		const bool reuseColor{ colors->size() == 1 };

		uint32_t count = 0;
		for (auto& vector : *vertices)
		{	
			if(!reuseColor)
				color = colors->at(count++);

			float vec[7] = { vector.x(), vector.y(), vector.z(), color.x(), color.y(), color.z(), color.w() };
			vertexBufferCreator.WriteVertexData(vec);
		}

		return vertexBufferCreator.CreateVertexAndIndexBuffer();
	}
}