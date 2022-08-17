#pragma once
#include "../../vulkan_manager/vulkan_manager.hpp"
#include "structs/vre_rendertarget_info.hpp"
#include "structs/drawable_list.hpp"
#include "structs/vre_pipelineset.hpp"
#include "vre_renderer.hpp"

namespace vre {
	class SceneReader;
}

namespace vre::Renderer::VreRenderer
{
	/// @brief The RenderList is directly used by the renderer and determines the order in which the drawables are rendered.
	/// The opaque drawables are always rendererd first, sorted by pipeline.
	/// The transparent drawable list is sorted by distance to the camera and updated when the camera moves.
	class VreRenderList
	{

	public:
		VreRenderList();
		VreRenderList(uint32_t width, uint32_t height, std::vector<VkImageView>* renderTargetImageViews, VkFormat format, float clearColorValues[4]);

		/// @brief Create all render objects for this renderlist.
		void Initialize();

		void Destroy();

		const VreRenderTargetInfo* GetRenderTargetInfo() { return &m_renderTargetInfo; }

		// object setter for external renderer manipulation
		void SetVreGraphicsPipelines(std::shared_ptr<VreGraphicsPipelinesAbstract> graphicsPipelines) { m_vreGraphicsPipelines = graphicsPipelines; }
		void SetVreAttachments(std::shared_ptr<VreAttachmentsAbstract> attachments) { m_vreAttachments = attachments; }
		void SetVreRenderPass(std::shared_ptr<VreRenderPassAbstract> renderPass) { m_vreRenderPass = renderPass; }
		void SetVreCommandBuffer(std::shared_ptr<VreCommandBufferAbstract> commandBuffer) { m_vreCommandBuffer = commandBuffer; }
		void SetVreFrameBuffer(std::shared_ptr<VreFrameBufferAbstract> frameBuffer) { m_vreFrameBuffer = frameBuffer; }

		std::shared_ptr<VreGraphicsPipelinesAbstract> GetVreGraphicsPipelines() { return m_vreGraphicsPipelines; }
		std::shared_ptr<VreAttachmentsAbstract> GetVreAttachments() { return m_vreAttachments; }
		std::shared_ptr<VreRenderPassAbstract> GetVreRenderPass() { return m_vreRenderPass; }
		std::shared_ptr<VreCommandBufferAbstract> GetVreCommandBuffer() { return m_vreCommandBuffer; }
		std::shared_ptr<VreFrameBufferAbstract> GetVreFrameBuffer() { return m_vreFrameBuffer; }

		/// @brief Each scene uses a subset of all shader combinations.
		/// @return The subset of shader combinations required for this renderlist/scene.
		//const std::list<std::shared_ptr<ShaderCombination>>* GetShaderCombinations() { return &m_shaderCombinations; }

		/// @brief Add a shader combination to this scene.
		/// @param shaderCombination 
		//void AddShaderCombination(std::shared_ptr<ShaderCombination> shaderCombination) { m_shaderCombinations.push_back(shaderCombination); }

		std::vector<DrawableList>* GetOpaqueDrawables() { return &m_opaqueDrawables; }
		std::vector<DrawableList>* GetTransparentDrawables() { return &m_transparentDrawables; }

		/// @brief The main scene is the only scene to render to the swapchain images and present them as a final result.
		/// @return 
		bool IsMainScene() { return m_isMainScene; }

		vre::SceneReader* GetSceneReader() { return m_sceneReader; }
		void SetSceneReader(vre::SceneReader* sceneReader) { m_sceneReader = sceneReader; }

		/// @brief Empty list of added pipelinesets. Called before recreating the pipelinesets.
		void ClearPipelineSets();

		void AddPipelineSet(uint64_t shaderCombinationIndex, std::shared_ptr<VrePipelineSet> pipelineSet);
		std::map<uint64_t, std::shared_ptr<VrePipelineSet>>* GetPipelineSets() { return &m_pipelineSets; }

	protected:

		/// @brief The scener reader that manages this render list.
		vre::SceneReader* m_sceneReader{ nullptr };

		/// @brief The scene a renderlist is created from. The scene contains rendering properties like height and width.
		std::vector<DrawableList> m_opaqueDrawables;
		std::vector<DrawableList> m_transparentDrawables;

		/// @brief Shader combinations used by this renderlist/scene.
		std::map<uint64_t, std::shared_ptr<VrePipelineSet>> m_pipelineSets;

		// each renderlist has a unique set of render objects, because the render target is individual per each.
		std::shared_ptr<VreGraphicsPipelinesAbstract> m_vreGraphicsPipelines;
		std::shared_ptr<VreAttachmentsAbstract> m_vreAttachments;
		std::shared_ptr<VreRenderPassAbstract> m_vreRenderPass;
		std::shared_ptr<VreCommandBufferAbstract> m_vreCommandBuffer;
		std::shared_ptr<VreFrameBufferAbstract> m_vreFrameBuffer;

		/// @brief The main scene is the only scene that uses the swapchain images as render target,
		/// to render the final result to the screen.
		bool m_isMainScene{ false };

		VreRenderTargetInfo m_renderTargetInfo{};

		void createSwapchainDependentObjects();
		void destroySwapchainDependentObjects();
		void swapchainRecreationCallback(bool swapchainSizeChanged);
	};
}