#pragma once
#include "../common.hpp"

namespace vre
{
	/// @brief A shader is identified by its file path. It can create a shader moudle based on the SPIR-V code.
	class Shader
	{
	public:

		/// @brief Setup a shader with the source file. 
		Shader(const std::string shaderSourceFile);

		/// @brief Get the vk shader module.
		VkShaderModule GetShaderModule();

		/// @brief Create the shader module.
		void CreateShaderModule();

		/// @brief Destroy the shader module.
		void DestroyShaderModule();

		/// @brief Get the path to the spir-v source.
		std::string GetShaderSourceFilePath() { return m_shaderSourceFile; }

		/// @brief Get the stage in the pipeline at which the shader is to be applied.
		virtual VkPipelineShaderStageCreateInfo GetShaderStageCreateInfo() = 0;

		/// @brief The unique 16 bit identifier of the shader.
		uint16_t GetShaderId() { return m_shaderId; }

	protected:
		/// @brief The handle to the shader module once created.
		VkShaderModule m_shaderModule{ VK_NULL_HANDLE };

		/// @brief Path to spir-v source.
		std::string m_shaderSourceFile;

		/// @brief String containing the shader code after reading the source file.
		std::string m_shaderCode;

		/// @brief Module create info.
		VkShaderModuleCreateInfo m_createInfo{};
		
		/// @brief The unique identifier for the shader.
		uint16_t m_shaderId{};
	};
}