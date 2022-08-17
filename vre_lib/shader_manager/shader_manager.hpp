#pragma once
#include "shader.hpp"
#include "shader_combination.hpp"
#include "uniform_buffer.hpp"
#include <string>
#include <unordered_map>

namespace vre
{
	/// @brief The shader manager returns shader combinations identified by shaders.
	class ShaderManager
	{
	public:
		/// @brief Static instance of the shader manager for public access to it.
		static ShaderManager* GetInstance();

		const std::unordered_map<uint64_t, std::weak_ptr<ShaderCombination>>* GetShaderCombinations() { return &m_shaderCombinations; }

		/// @brief Returns a unique shader combination based on the input shaders. Each unique shader combination
		/// needs its own pipeline.
		/// @param vertexShader - The vertex shader of the combination.
		/// @param fragmentShader - The fragment shader of the combination.
		/// @param dsl - The descriptor set layout.
		/// @return A unique shader combination.
		std::shared_ptr<ShaderCombination> GetShaderCombination(std::shared_ptr<VertexShader> vertexShader, std::shared_ptr<FragmentShader> fragmentShader, std::shared_ptr<DescriptorSetLayout> dsl);

	private:
		/// @brief A map to all created shader combinations.
		std::unordered_map<uint64_t, std::weak_ptr<ShaderCombination>> m_shaderCombinations;	
	};
}