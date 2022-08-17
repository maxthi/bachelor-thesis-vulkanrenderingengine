#pragma once
#include "../shader.hpp"
#include "../../common.hpp"
#include "../vertex_layout.hpp"

namespace vre
{
	/// @brief Vertex shaders inheriting from shader and allowing to setup a vertex shader with a vertex layout.
	class VertexShader : public Shader
	{
	public:
		/// @brief Get a vertex shader.
		/// @param shaderSourceFile - The spirv source file.
		/// @param layout - The vertex layout.
		/// @return - A shared ptr to the vertex shader.
		static std::shared_ptr<VertexShader> GetVertexShader(const std::string& shaderSourceFile, std::shared_ptr<VertexLayout> layout);

		/// @brief Get shader stage create info.
		virtual VkPipelineShaderStageCreateInfo GetShaderStageCreateInfo();

		/// @brief Get the attached vertex layout.
		/// @return 
		std::shared_ptr<VertexLayout> GetVertexLayout() { return m_pVertexShaderLayout; }
	
		/// @brief Constructor for vertex shaders, but GetVertexShader is to be used instead.
		VertexShader(const std::string& shaderSourceFile, std::shared_ptr<VertexLayout> layout);
	private:
		/// @brief Static map to identify shaders with the same source file.
		static inline std::unordered_map<std::string, std::shared_ptr<VertexShader>> s_uniqueShaders{};

		/// @brief The 16 bit identifier to identify shaders between another.
		static inline uint16_t s_shaderId{ 0 };

		/// @brief The vertex shader layout to be used.
		std::shared_ptr<VertexLayout> m_pVertexShaderLayout;
	};
}