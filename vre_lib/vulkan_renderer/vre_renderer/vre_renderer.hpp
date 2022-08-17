#pragma once
#include "../../vulkan_manager/vulkan_manager.hpp"
#include "../renderer.hpp"

namespace vre::Renderer::VreRenderer
{
	class VreAttachments;
	class VreRenderPass;
	class VreGraphicsPipelines;
	class VreCommandBuffer;
	class VreFrameBuffer;
	class VreCommandPool;
	class VreDrawer;
	class VreRenderList;

	class VreAttachmentsAbstract;
	class VreRenderPassAbstract;
	class VreGraphicsPipelinesAbstract;
	class VreCommandBufferAbstract;
	class VreFrameBufferAbstract;
	class VreCommandPoolAbstract;
	class VreDrawerAbstract;

	class VreRenderer : public Renderer
	{

	public:
		/// @brief The renderer instance for all renderer objects to interact together.
		/// @return The instance of the renderer 
		static VreRenderer* GetInstance();

		void Initialize() override;
		void Destroy() override;
		void DrawFrame() override;

		/// @brief Called extern if the transparent objects in the render list have changed
		/// and the primary command buffers need to be rerecorded
		void TransparentDrawablesChanged();

		/// @brief Called extern if the opaque objects in the render list have changed
		/// and the secondary command buffers need to be rerecorded
		void OpaqueDrawablesChanged();

		/// @brief Set the list of render lists.
		/// @param listOfRenderLists - a list containing render lists
		void SetRenderLists(std::shared_ptr<std::list<std::shared_ptr<VreRenderList>>> listOfRenderLists) { m_listOfRenderLists = listOfRenderLists; }

		/// @brief Returns a list containing all render lists to be rendered by this renderer.
		std::shared_ptr<std::list<std::shared_ptr<VreRenderList>>> GetRenderLists() { return m_listOfRenderLists; }

		// object setter for external renderer manipulation
		void SetVreCommandPool(std::shared_ptr<VreCommandPoolAbstract> commandPool) { m_vreCommandPool = commandPool; }
		void SetVreDrawer(std::shared_ptr<VreDrawerAbstract> drawer) { m_vreDrawer = drawer; }

		std::shared_ptr<VreCommandPoolAbstract> GetVreCommandPool() { return m_vreCommandPool; }
		std::shared_ptr<VreDrawerAbstract> GetVreDrawer() { return m_vreDrawer; }

		void RecordCommandBuffer(uint32_t imageIndex, bool recordTransparent, bool recordOpaque);
		const std::vector<VkCommandBuffer>* GetCommandBuffers();

	private:
		VreRenderer() {};
		~VreRenderer();

		bool m_destroyed{ false };

		std::shared_ptr<VreCommandPoolAbstract> m_vreCommandPool;
		std::shared_ptr<VreDrawerAbstract> m_vreDrawer;

		/// @brief A sorted list of render lists. Start rendering with the first item in the list.
		std::shared_ptr<std::list<std::shared_ptr<VreRenderList>>> m_listOfRenderLists;

	};
}