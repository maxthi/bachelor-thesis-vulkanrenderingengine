#pragma once
#include <project_relevant_headers.hpp>

// A3)
// Man sieht nichts solange die descriptions nicht richtig konfiguriert sind
namespace vre
{
	class VertexLayoutARTR : public VertexLayout
	{
		// lokales struct welches die Struktur der Vertexdaten beschreibt, um sizeof() und das offsetof() makro anzuwenden.
		struct Vertex
		{
			float inMixed1[3];
			float inMixed2[3];
		};

	public:
		VertexLayoutARTR()
		{
			VkVertexInputBindingDescription bindingDescription{};
			// TODO: bindingDescription values konfigurieren..
			m_bindingDescriptions.push_back(bindingDescription);

			VkVertexInputAttributeDescription attributeDescription{};
			// TODO: erste attribute description entsprechend befüllen ...
			m_attributeDescriptions.push_back(attributeDescription);

			// TODO: attribute description ein zweites mal befüllen ...
			m_attributeDescriptions.push_back(attributeDescription);
		}

		std::shared_ptr<VertexAndIndexBuffer> CreateVertexAndIndexBuffer(std::shared_ptr<Drawable> drawable) override
		{
			// über das als parameter übergebene drawable kann auf die properties zugegriffen werden,
			// z.b. die property mesh
			std::shared_ptr<Mesh> mesh = drawable->GetProperty<Mesh>();

			// über das mesh kann auf die von uns in der main.cpp definierten vertex & color buffer zugegriffen werden
			std::vector<QVector3D>* vertices = mesh->GetVertices();
			std::vector<QVector4D>* colors = mesh->GetColors();

			// erstellt einen vertex buffer creator, der den vertex buffer autmatisch erstellt.
			// stride beschreibt die größe für einen satz an vertex daten. floats als position und 3 floats als farbinformation
			uint8_t stride = 6;
			VertexBufferCreator<float> vertexBufferCreator(mesh, stride);

			for (int i = 0; i < vertices->size(); i++)
			{
				// TODO: einkommentieren und data[6] array befüllen
				//float data[6] = { ... };
				//vertexBufferCreator.WriteVertexData(data);

				//  Hinweis: auf lokale variablen vertices und colors zugreifen, vertices->at(i).x() , colors->at(i).x(), ... etc
				//  und richtig anordnen
			}

			return vertexBufferCreator.CreateVertexAndIndexBuffer();
		}

		// shader aus komponenten zusammen bauen
		static std::shared_ptr<Shaders<UniformBufferColor>> GetShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
		{
			auto fragmentShader = FragmentShader::GetFragmentShader(fragmentShaderFilePath);
			auto vertexShader = VertexShader::GetVertexShader(vertexShaderFilePath, std::make_shared<VertexLayoutARTR>());
			auto shaderCombination = ShaderManager::GetInstance()->GetShaderCombination(vertexShader, fragmentShader, UniformBufferColor::GetDescriptorSetLayout());
			return std::make_shared<Shaders<UniformBufferColor>>(shaderCombination);
		}

		~VertexLayoutARTR() = default;
	};
}