#pragma once
#include "../shader.hpp"
#include "../../common.hpp"

namespace vre
{
	class FragmentShader : public Shader
	{
	public:
		/// @brief Get a fragment shader.
		/// @param shaderSourceFile - The spir-v source file identifying the shader.
		static std::shared_ptr<FragmentShader> GetFragmentShader(const std::string& shaderSourceFile);

		FragmentShader(const std::string& shaderSourceFile);

		/// @brief Get shader stage create info.
		virtual VkPipelineShaderStageCreateInfo GetShaderStageCreateInfo();

	private:
		/// @brief Static map to identify shaders with the same source file.
		static inline std::unordered_map<std::string, std::shared_ptr<FragmentShader>> s_uniqueShaders{};

		/// @brief The 16 bit identifier to identify shaders between another.
		static inline uint16_t s_shaderId{ 0 };
	};
}