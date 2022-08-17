#pragma once
#include "../shader.hpp"
#include "../../common.hpp"

namespace vre
{
	class GeometryShader : public Shader
	{
	public:
		/// @brief Get a unique geometry shader.
		/// @param shaderSourceFile - Spir-v source file used as identifier.
		static std::shared_ptr<GeometryShader> GetGeometryShader(const std::string& shaderSourceFile);

		GeometryShader(const std::string& shaderSourceFile);

		/// @brief Get shader stage create info.
		virtual VkPipelineShaderStageCreateInfo GetShaderStageCreateInfo();

	private:
		/// @brief Static map to identify shaders with the same source file.
		static inline std::unordered_map<std::string, std::shared_ptr<GeometryShader>> s_uniqueShaders{};

		/// @brief The 16 bit identifier to identify shaders between another.
		static inline uint16_t s_shaderId{ 0 };
	};
}