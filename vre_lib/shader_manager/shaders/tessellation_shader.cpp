#include "tessellation_shader.hpp"

namespace vre
{
	std::shared_ptr<TessellationShader> TessellationShader::GetTessellationShader(const std::string& shaderSourceFile)
	{
		auto shaderIt = s_uniqueShaders.find(shaderSourceFile);
		if (shaderIt != s_uniqueShaders.end())
		{
			return shaderIt->second;
		}
		auto shader = std::make_shared<TessellationShader>(shaderSourceFile);
		s_uniqueShaders.insert({ shaderSourceFile, shader });
		return shader;
	}

	TessellationShader::TessellationShader(const std::string& shaderSourceFile) :
		Shader(shaderSourceFile)
	{
		m_shaderId = s_shaderId++;
	}

	VkPipelineShaderStageCreateInfo TessellationShader::GetShaderStageCreateInfo()
	{
		VkPipelineShaderStageCreateInfo shaderStageInfo{};
		shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStageInfo.stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
		shaderStageInfo.module = m_shaderModule;
		shaderStageInfo.pName = "main";
		return shaderStageInfo;
	}

}