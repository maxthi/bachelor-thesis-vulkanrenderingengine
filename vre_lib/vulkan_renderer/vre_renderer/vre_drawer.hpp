#pragma once
#include "vre_renderer.hpp"

namespace vre::Renderer::VreRenderer
{
	class VreDrawerAbstract : public RendererObject
	{
	public:
		virtual void DrawFrame() {};
	};

	/// @brief The drawer is the class that holds the actual draw frame method
	/// within the swap chain images are handled/synchronized and the drawing commands are executed
	class VreDrawer : public VreDrawerAbstract
	{
	public:
		virtual void Create() override;
		virtual void Destroy() override;

		virtual void DrawFrame() override;

		/// @brief For drawing a lot of vulkan objects are necessary. Their handles are stored within the class for quick access.
		/// Whenever the vulkan objects get updated the handles to the objects need to be refreshed as well.
		void UpdateVulkanObjects();

		// @brief Called extern if the transparent objects in the render list have changed
		/// and the primary command buffers need to be rerecorded
		void TransparentDrawablesChanged();

		/// @brief Called extern if the opaque objects in the render list have changed
		/// and the secondary command buffers need to be rerecorded
		void OpaqueDrawablesChanged();

	protected:

		// render list of the renderer
		std::shared_ptr<std::list<std::shared_ptr<VreRenderList>>> m_listOfRenderLists{};

		/// @brief The render list that renders to the swapchain and holds the command buffer that is the starting point for drawing.
		std::weak_ptr<VreRenderList> m_mainScene;


		size_t m_swapchainSize{};

		// rendering objects
		std::vector<VkSemaphore> m_imageAvailableSemaphores;
		std::vector<VkSemaphore> m_renderFinishedSemaphores;
		std::vector<VkFence> m_fences;
		std::vector<VkFence> m_imagesInFlight;

		const int32_t m_maximumFramesInFlight = 2;
		int32_t m_currentFrame = 0;

		// vulkan handles needed for rendering, stored for direct access.
		// updated by updateVulkanObjects();
		VkDevice m_device{};
		VkSwapchainKHR m_swapchain{};
		const std::vector<VkCommandBuffer>* m_commandBuffers{};
		VkQueue m_graphicsQueue{};
		VkQueue m_presentQueue{};

		/// @brief A list to check for each primary command buffer if rerecording has to be done.
		std::vector<bool> m_recordCommandBuffersTransparentObjects;

		/// @brief A list to check for each secondary command buffer if rerecording has to be done.
		std::vector<bool> m_recordCommandBuffersOpaqueObjects;

		virtual void updateUniformBuffers(uint32_t imageIndex);

		virtual void recreateUniformBuffers();

		virtual void recreateSwapchain();
	};
}