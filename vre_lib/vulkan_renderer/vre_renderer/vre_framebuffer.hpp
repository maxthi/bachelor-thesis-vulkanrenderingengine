#pragma once
#include "vre_renderer.hpp"
#include "vre_renderlist.hpp"

namespace vre::Renderer::VreRenderer
{
	class VreFrameBufferAbstract : public RendererObject
	{
	public:
		virtual const std::vector<VkFramebuffer>* GetFrameBuffers() { return nullptr; }
	};

	class VreFrameBuffer : public VreFrameBufferAbstract
	{
	public:
		VreFrameBuffer(VreRenderList* vreRenderList) { m_vreRenderList = vreRenderList; }
		void Create() override;
		void Destroy() override;

		const std::vector<VkFramebuffer>* GetFrameBuffers() override;

	private:
		VreRenderList* m_vreRenderList{};
		std::vector<VkFramebuffer> m_framebuffers;
	};
}