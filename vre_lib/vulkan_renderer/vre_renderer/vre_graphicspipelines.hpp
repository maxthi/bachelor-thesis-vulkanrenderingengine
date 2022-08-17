#pragma once
#include "vre_renderer.hpp"
#include "structs/drawable_list.hpp"

namespace vre::Renderer::VreRenderer
{
	class VreGraphicsPipelinesAbstract : public RendererObject
	{
	};

	/// @brief A pipeline for each shader module
	class VreGraphicsPipelines : public VreGraphicsPipelinesAbstract
	{
	public:
		VreGraphicsPipelines(VreRenderList* vreRenderList) { m_vreRenderList = vreRenderList; }

		void Create() override;
		void Destroy() override;

	protected:
		VreRenderList* m_vreRenderList{};

        void createPipeline(
			DrawableList& drawableList,
			VkPipelineVertexInputStateCreateInfo& vertexInputInfo,
			VkPipelineLayoutCreateInfo& pipelineLayoutInfo,
			VkGraphicsPipelineCreateInfo& pipelineInfo
		);
	};
}
