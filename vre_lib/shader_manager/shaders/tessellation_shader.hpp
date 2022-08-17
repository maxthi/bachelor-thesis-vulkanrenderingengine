#pragma once
#include "../shader.hpp"
#include "../../common.hpp"

namespace vre
{
	class TessellationShader : public Shader
	{
	public:
		/// @brief Get a unique tessellation shader.
		/// @param shaderSourceFile - spir-v source to identify shader.
		static std::shared_ptr<TessellationShader> GetTessellationShader(const std::string& shaderSourceFile);

		TessellationShader(const std::string& shaderSourceFile);

		/// @brief Get shader stage create info.
		virtual VkPipelineShaderStageCreateInfo GetShaderStageCreateInfo();
	private:

		/// @brief Static map to identify shaders with the same source file.
		static inline std::unordered_map<std::string, std::shared_ptr<TessellationShader>> s_uniqueShaders{};

		/// @brief The 16 bit identifier to identify shaders between another.
		static inline uint16_t s_shaderId{ 0 };
	};
}