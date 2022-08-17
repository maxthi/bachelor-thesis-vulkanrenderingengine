#pragma once
#include "vre_renderer.hpp"
#include "vre_renderlist.hpp"
#include "structs/drawable_list.hpp"

namespace vre::Renderer::VreRenderer
{
	/// @brief The abstract class allows to exchange the command buffer object for the vre renderer.
	class VreCommandBufferAbstract : public RendererObject
	{
	public:
		virtual void RecordCommandBuffer(uint32_t imageIndex, bool recordTransparent, bool recordOpaque) {}
		virtual const std::vector<VkCommandBuffer>* GetCommandBuffers() { return nullptr; }
		virtual void RecordRenderPass(VkCommandBuffer primaryCommandBuffer, uint32_t imageIndex) {};
		virtual void RequestRerecording(bool opaqueDrawables, bool transparentDrawables) {};
		virtual bool IsRerecordingRequested(uint32_t imageIndex) { return false; };
	};

	class VreCommandBuffer : public VreCommandBufferAbstract
	{
	public:
		VreCommandBuffer(VreRenderList* vreRenderList) { m_vreRenderList = vreRenderList; }

		void Create() override;
		void Destroy() override;

		void RecordCommandBuffer(uint32_t imageIndex, bool recordTransparent, bool recordOpaque) override;

		const std::vector<VkCommandBuffer>* GetCommandBuffers() override;

		void RecordRenderPass(VkCommandBuffer primaryCommandBuffer, uint32_t imageIndex) override;

		/// @brief Set request rerecording to true. The drawer checks the command buffers for each
		void RequestRerecording(bool opaqueDrawables, bool transparentDrawables);

		bool IsRerecordingRequested(uint32_t imageIndex) { return m_rerecordingRequested.at(imageIndex); }

	protected:

		VreRenderList* m_vreRenderList{};

		std::array<VkClearValue, 2> m_clearValues{};

		/// @brief A pointer to the command buffers used. It is set in the Create method.
		const std::vector<VkFramebuffer>* m_framebuffers{};

		VkRenderPassBeginInfo m_renderPassBeginInfo{};

		/// @brief Commandbuffers that contain the opaque objects and only update if new objects are added/removed from a scene.
		std::vector<VkCommandBuffer> m_commandBuffersOpaqueObjects;

		/// @brief Commandbufffers for the the transparent objects.
		std::vector<VkCommandBuffer> m_commandBuffersTransparentObjects;

		/// @brief The primary command buffer only executes the secondary command buffers
		std::vector<VkCommandBuffer> m_commandBuffersPrimary;

		std::vector<bool> m_rerecordingRequested{};

		void recordPrimaryCommandBuffer(uint32_t imageIndex, VkRenderPass renderPass, VkFramebuffer framebuffer, const VkExtent2D* pExtent);

		/// @brief Recording the draw commands for a command buffer
		/// @param commandBuffers - The set of commands buffers this command buffer belongs to. 
		/// @param drawableList - The list of drawables that shall be drawn in this command buffer.
		/// @param i - The index of the exact command buffer to record.
		/// @param renderPass - The render pass
		/// @param framebuffer - The framebuffer this command buffer uses.
		void recordCommandBuffer(
			const std::vector<VkCommandBuffer>& commandBuffers,
			const std::vector<DrawableList>* drawableList,
			uint32_t index,
			VkRenderPass renderPass,
			VkFramebuffer framebuffer
		);

		void allocateCommandBuffers(std::vector<VkCommandBuffer>& commandBuffers, VkCommandBufferLevel cmdBufferLevel);

		/// @brief Setup constant member variables used during command buffer recording, like the clear values,
		/// the renderpass dimensions and the framebuffer pointer.
		void setupRenderPassInfo();
	};
}