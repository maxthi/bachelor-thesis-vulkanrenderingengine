#include "geometry_shader.hpp"

namespace vre
{
	std::shared_ptr<GeometryShader> GeometryShader::GetGeometryShader(const std::string& shaderSourceFile)
	{
		auto shaderIt = s_uniqueShaders.find(shaderSourceFile);
		if (shaderIt != s_uniqueShaders.end())
		{
			return shaderIt->second;
		}
		auto shader = std::make_shared<GeometryShader>(shaderSourceFile);
		s_uniqueShaders.insert({ shaderSourceFile, shader });
		return shader;
	}

	GeometryShader::GeometryShader(const std::string& shaderSourceFile) :
		Shader(shaderSourceFile)
	{
		m_shaderId = s_shaderId++;
	}

	VkPipelineShaderStageCreateInfo GeometryShader::GetShaderStageCreateInfo()
	{
		VkPipelineShaderStageCreateInfo shaderStageInfo{};
		shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStageInfo.stage = VK_SHADER_STAGE_GEOMETRY_BIT;
		shaderStageInfo.module = m_shaderModule;
		shaderStageInfo.pName = "main";
		return shaderStageInfo;
	}
}