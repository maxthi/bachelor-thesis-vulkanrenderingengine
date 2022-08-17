#pragma once
#include "../shader.hpp"
#include "../../common.hpp"

namespace vre
{
	class ComputeShader : public Shader
	{
	public:
		/// @brief Get a compute shader.
		/// @param shaderSourceFile - The shader source file.
		static std::shared_ptr<ComputeShader> GetComputeShader(const std::string& shaderSourceFile);

		ComputeShader(const std::string& shaderSourceFile);

		/// @brief Get shader stage create info.
		virtual VkPipelineShaderStageCreateInfo GetShaderStageCreateInfo();

	private:
		/// @brief Static map to identify shaders with the same source file.
		static inline std::unordered_map<std::string, std::shared_ptr<ComputeShader>> s_uniqueShaders{};

		/// @brief The 16 bit identifier to identify shaders between another.
		static inline uint16_t s_shaderId{ 0 };
	};
}