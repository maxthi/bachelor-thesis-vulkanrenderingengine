#include "vl_texture.hpp"
#include "../../scene_manager/scene_objects/drawable.hpp"
#include "../../scene_manager/drawable_properties/mesh.hpp"

namespace vre
{
	VertexLayoutTexture::VertexLayoutTexture()
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
		attributeDescription.format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescription.offset = offsetof(Vertex, texCoords);
		m_attributeDescriptions.push_back(attributeDescription);
	}

	std::shared_ptr<VertexAndIndexBuffer> VertexLayoutTexture::CreateVertexAndIndexBuffer(std::shared_ptr<Drawable> drawable)
	{		
		// arranging vertex positions and tex coords together in a uint32_t buffer
		std::shared_ptr<Mesh> mesh = drawable->GetProperty<Mesh>();
		auto vertices = mesh->GetVertices();
		auto texCoords = mesh->GetTexCoords();

		// 3 floats per position, 4 floats per color
		uint8_t stride = 5;
		VertexBufferCreator<float> vertexBufferCreator(mesh, stride);

		uint32_t count = 0;
		for (auto& vector : *vertices)
		{

			auto texCoord = texCoords->at(count++);
			float vec[5] = { vector.x(), vector.y(), vector.z(), texCoord.x(), texCoord.y() };
			vertexBufferCreator.WriteVertexData(vec);
		}

		return vertexBufferCreator.CreateVertexAndIndexBuffer();
	}
}