#include "fragment_shader.hpp"

namespace vre
{
	FragmentShader::FragmentShader(const std::string& shaderSourceFile) :
		Shader(shaderSourceFile)
	{
		m_shaderId = s_shaderId++;
	}

	std::shared_ptr<FragmentShader> FragmentShader::GetFragmentShader(const std::string& shaderSourceFile)
	{
		auto shaderIt = s_uniqueShaders.find(shaderSourceFile);
		if (shaderIt != s_uniqueShaders.end())
		{
			return shaderIt->second;
		}
		auto shader = std::make_shared<FragmentShader>(shaderSourceFile);
		s_uniqueShaders.insert({ shaderSourceFile, shader });
		return shader;
	}

	VkPipelineShaderStageCreateInfo FragmentShader::GetShaderStageCreateInfo()
	{
		VkPipelineShaderStageCreateInfo shaderStageInfo{};
		shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shaderStageInfo.module = m_shaderModule;
		shaderStageInfo.pName = "main";
		return shaderStageInfo;
	}
}