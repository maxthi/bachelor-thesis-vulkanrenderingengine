#include "vl_phong.hpp"
#include <project_relevant_headers.hpp>

namespace vre
{
	VertexLayoutPhong::VertexLayoutPhong()
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

		attributeDescription.binding = 0;
		attributeDescription.location = 2;
		attributeDescription.format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescription.offset = offsetof(Vertex, normal);
		m_attributeDescriptions.push_back(attributeDescription);
	}

	std::shared_ptr<VertexAndIndexBuffer> VertexLayoutPhong::CreateVertexAndIndexBuffer(std::shared_ptr<Drawable> drawable)
	{
		
		// arranging vertex positions and tex coords together in a uint32_t buffer
		std::shared_ptr<Mesh> mesh = drawable->GetProperty<Mesh>();
		auto vertices = mesh->GetVertices();
		auto texCoords = mesh->GetTexCoords();
		auto normals = mesh->GetNormals();

		// 3 floats per position, 4 floats per color
		uint8_t stride = 8;
		VertexBufferCreator<float> vertexBufferCreator(mesh, stride);

		uint32_t count = 0;
		for (auto& vector : *vertices)
		{

			auto texCoord = texCoords->at(count);
			auto normal = normals->at(count);
			// put vertex data into order
			float data[8] = { vector.x(), vector.y(), vector.z(), texCoord.x(), texCoord.y(), normal.x(), normal.y(), normal.z() };
			vertexBufferCreator.WriteVertexData(data);
			count++;
		}

		return vertexBufferCreator.CreateVertexAndIndexBuffer();
	}
}