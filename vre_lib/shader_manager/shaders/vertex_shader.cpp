#include "vertex_shader.hpp"

namespace vre
{
	VertexShader::VertexShader(const std::string& shaderSourceFile, std::shared_ptr<VertexLayout> layout) :
		Shader(shaderSourceFile), m_pVertexShaderLayout(layout)
	{
		m_shaderId = s_shaderId++;
	}

	std::shared_ptr<VertexShader> VertexShader::GetVertexShader(const std::string& shaderSourceFile, std::shared_ptr<VertexLayout> layout)
	{
		auto shaderIt = s_uniqueShaders.find(shaderSourceFile);
		if (shaderIt != s_uniqueShaders.end())
		{
			return shaderIt->second;
		}
		auto vertexShader = std::make_shared<VertexShader>(shaderSourceFile, layout);
		s_uniqueShaders.insert({ shaderSourceFile, vertexShader });
		return vertexShader;
	}

	VkPipelineShaderStageCreateInfo VertexShader::GetShaderStageCreateInfo()
	{
		checkPtr(m_shaderModule, "VertexShader::GetShaderStageCreateInfo: shader module is null");
		VkPipelineShaderStageCreateInfo shaderStageInfo{};
		shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		shaderStageInfo.module = m_shaderModule;
		shaderStageInfo.pName = "main";
		return shaderStageInfo;
	}
}