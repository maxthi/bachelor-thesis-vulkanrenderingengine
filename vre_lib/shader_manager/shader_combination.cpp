#include "shader_combination.hpp"
#include "shader_manager.hpp"
#include "../vulkan_manager/vulkan_manager.hpp"

namespace vre
{
	void ShaderCombination::SetVertexShader(std::shared_ptr<VertexShader> vertexShader)
	{
		checkComputeShader();
		auto shaderId = vertexShader->GetShaderId();
		// clear old index
		m_scIndex &= 0xFFFFFFFFFFFF0000;
		m_scIndex |= shaderId;
		m_vertexShader = vertexShader;
	}

	void ShaderCombination::SetFragmentShader(std::shared_ptr<FragmentShader> fragmentShader)
	{
		checkComputeShader();
		// clear old index
		m_scIndex &= 0xFFFFFFFF0000FFFF;
		// write new
		uint64_t fragmentId = fragmentShader->GetShaderId();
		fragmentId <<= 16;
		m_scIndex |= fragmentId;
		m_fragmentShader = fragmentShader;
	}

	void ShaderCombination::SetGeometryShader(std::shared_ptr<GeometryShader> geometryShader)
	{
		checkComputeShader();
		// clear old index
		m_scIndex &= 0xFFFF0000FFFFFFFF;
		// write new
		uint64_t geometryId = geometryShader->GetShaderId();
		geometryId <<= 32;
		m_scIndex |= geometryId;
		m_geometryShader = geometryShader;
	}

	void ShaderCombination::SetTessellationShader(std::shared_ptr<TessellationShader> tessellationShader)
	{
		checkComputeShader();
		// clear old index
		m_scIndex &= 0x0000FFFFFFFFFFFF;
		// write new
		uint64_t tessellationId = tessellationShader->GetShaderId();
		tessellationId <<= 48;
		m_scIndex |= tessellationId;
		m_tesselationShader = tessellationShader;
	}

	void ShaderCombination::SetComputeShader(std::shared_ptr<ComputeShader> computeShader)
	{
		if (m_vertexShader != nullptr
			|| m_fragmentShader != nullptr
			|| m_geometryShader != nullptr
			|| m_tesselationShader != nullptr)
		{
			err("ShaderCombination::SetComputeShader: Can only set a compute shader when no other shader is set to this shader combination!");
		}
		uint64_t computeId = computeShader->GetShaderId();
		computeId <<= 48;
		m_scIndex = computeId;
		m_computeShader = computeShader;
	}

	std::vector<VkPipelineShaderStageCreateInfo> ShaderCombination::GetShaderStageCreateInfos()
	{
		std::vector<VkPipelineShaderStageCreateInfo> createInfos;
		if (m_vertexShader != nullptr)
		{
			createInfos.push_back(m_vertexShader->GetShaderStageCreateInfo());
		}

		if (m_fragmentShader != nullptr)
		{
			createInfos.push_back(m_fragmentShader->GetShaderStageCreateInfo());
		}

		if (m_geometryShader != nullptr)
		{
			createInfos.push_back(m_geometryShader->GetShaderStageCreateInfo());
		}

		if (m_tesselationShader != nullptr)
		{
			createInfos.push_back(m_tesselationShader->GetShaderStageCreateInfo());
		}

		if (m_computeShader != nullptr)
		{
			createInfos.push_back(m_computeShader->GetShaderStageCreateInfo());
		}
		
		if (createInfos.size() == 0)
		{
			err("ShaderCombination::GetShaderStageCreateInfos: There must be a least on shader in this shader combination!");
		}

		return createInfos;
	}

	void ShaderCombination::CreateShaderModules()
	{
		if (m_vertexShader != nullptr)
		{
			m_vertexShader->CreateShaderModule();
		}
		if (m_fragmentShader != nullptr)
		{
			m_fragmentShader->CreateShaderModule();
		}
		if (m_geometryShader != nullptr)
		{
			m_geometryShader->CreateShaderModule();
		}
		if (m_tesselationShader != nullptr)
		{
			m_tesselationShader->CreateShaderModule();
		}
		if (m_computeShader != nullptr)
		{
			m_computeShader->CreateShaderModule();
		}
	}

	void ShaderCombination::DestroyShaderModules()
	{
		if (m_vertexShader != nullptr)
		{
			m_vertexShader->DestroyShaderModule();
		}
		if (m_fragmentShader != nullptr)
		{
			m_fragmentShader->DestroyShaderModule();
		}
		if (m_geometryShader != nullptr)
		{
			m_geometryShader->DestroyShaderModule();
		}
		if (m_tesselationShader != nullptr)
		{
			m_tesselationShader->DestroyShaderModule();
		}
		if (m_computeShader != nullptr)
		{
			m_computeShader->DestroyShaderModule();
		}
	}
	
	void ShaderCombination::checkComputeShader()
	{
		if (m_computeShader != nullptr)
		{
			err("ShaderCombination::SetVertexShader: Cannot set a vertex shader when a compute shader is already present!");
		}
	}
}