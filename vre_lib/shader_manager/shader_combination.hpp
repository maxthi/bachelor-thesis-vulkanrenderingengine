/*
	Shader combination

	contains a shared ptr for each shader
	used by default shader simply as property.
	used by everything else to access all shader properties, like attribute bindings etc.


*/

#pragma once
#include "../common.hpp"
#include "shaders/vertex_shader.hpp"
#include "shaders/fragment_shader.hpp"
#include "shaders/geometry_shader.hpp"
#include "shaders/compute_shader.hpp"
#include "shaders/tessellation_shader.hpp"
#include "uniform_buffer.hpp"
#include "descriptor_set_layout.hpp"

namespace vre
{
	class ShaderManager;
	/// @brief A ShaderStageCombination contains ShaderStageCreateInfos and is used directly at pipeline creation.
	/// ShaderStages have to be added carefully because there is no test if a shader stage with an equal stage bit has been added
	class ShaderCombination
	{
	public:
		void SetVertexShader(std::shared_ptr<VertexShader> vertexShader);
		std::shared_ptr<VertexShader> GetVertexShader() { return m_vertexShader; }

		void SetFragmentShader(std::shared_ptr<FragmentShader> fragmentShader);
		std::shared_ptr<FragmentShader> GetFragmentShader() { return m_fragmentShader; }

		void SetGeometryShader(std::shared_ptr<GeometryShader> geometryShader);
		std::shared_ptr<GeometryShader> GetGeometryShader() { return m_geometryShader; }

		void SetTessellationShader(std::shared_ptr<TessellationShader> tessellationShader);
		std::shared_ptr<TessellationShader> GetTessellationShader() { return m_tesselationShader; }

		void SetComputeShader(std::shared_ptr<ComputeShader> computeShader);
		std::shared_ptr<ComputeShader> GetComputeShader() { return m_computeShader; }

		/// @brief The scIndex uniquely identifies a shader combination and its subshaders.
		/// A 64 bit integer where 16 bit describe one of the 4 shaders respectively, see below for a detailed commentary.
		/// @return The Index.
		uint64_t GetShaderCombinationIndex() { return m_scIndex; }

		/// @brief Returns a vector containing the shader stage create infos of all added shaders.
		/// Used during pipeline creation.
		std::vector<VkPipelineShaderStageCreateInfo> GetShaderStageCreateInfos();

		void SetDescriptorSetLayout(std::shared_ptr<DescriptorSetLayout> descriptorSetLayout) { m_descriptorSetLayout = descriptorSetLayout; }
		std::shared_ptr<DescriptorSetLayout> GetDescriptorSetLayout() { return m_descriptorSetLayout; }

		/// @brief Create shader modules before they are used in the pipeline.
		void CreateShaderModules();

		/// @brief Destroy shader modules after usage in pipeline creation.
		void DestroyShaderModules();

		ShaderCombination() = default;
		~ShaderCombination() = default;
	private:

		std::shared_ptr<VertexShader> m_vertexShader;
		std::shared_ptr<FragmentShader> m_fragmentShader;
		std::shared_ptr<GeometryShader> m_geometryShader;
		std::shared_ptr<TessellationShader> m_tesselationShader;
		std::shared_ptr<ComputeShader> m_computeShader;

		/// @brief A 64 bit integer containing the 16 bit ids of all 4 shaders or the compute shader.
		/// Structure: Bits:    64-48    |   47-32  |   31-16  |  15-0
		///            Ids:  tesselation | geometry | fragment | vertex
		///                    compute
		uint64_t m_scIndex{};

		/// @brief Check if a compute shader has been set for the shader combination and throw an error if so.
		/// A shader combination can be either a combination of 4 shaders or one compute shader.
		void checkComputeShader();

		/// @brief The uniform buffers used by this shader combination.
		std::shared_ptr<DescriptorSetLayout> m_descriptorSetLayout;

	};
}