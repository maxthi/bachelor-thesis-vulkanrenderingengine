#include "shader_manager.hpp"

namespace vre
{
	ShaderManager* ShaderManager::GetInstance()
	{
		static ShaderManager shaderManager;
		return &shaderManager;
	}

	std::shared_ptr<ShaderCombination> ShaderManager::GetShaderCombination(std::shared_ptr<VertexShader> vertexShader, std::shared_ptr<FragmentShader> fragmentShader, std::shared_ptr<DescriptorSetLayout> dsl)
	{
		// setting the vertex and fragment shaders sets the shader combination id which we can use
		// to query for an exisiting shader combination
		auto shaderCombination = std::make_shared<ShaderCombination>();
		shaderCombination->SetVertexShader(vertexShader);
		shaderCombination->SetFragmentShader(fragmentShader);
		shaderCombination->SetDescriptorSetLayout(dsl);
		
		// use new objects shader id to query if we already have an object with that id
		auto shaderCombinationIt = m_shaderCombinations.find(shaderCombination->GetShaderCombinationIndex());
		if (shaderCombinationIt != m_shaderCombinations.end())
		{
			// check if weak ptr is still a valid object
			auto storedShaderCombination = shaderCombinationIt->second.lock();
			if (storedShaderCombination != nullptr)
			{
				return storedShaderCombination;
			}
		}
		
		// keep new object and store it
		m_shaderCombinations.insert({ shaderCombination->GetShaderCombinationIndex(), shaderCombination });
		return shaderCombination;
	}
}