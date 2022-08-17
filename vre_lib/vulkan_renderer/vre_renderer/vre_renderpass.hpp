#pragma once
#include "vre_renderer.hpp"
#include "vre_renderlist.hpp"

namespace vre::Renderer::VreRenderer
{
	class VreRenderPassAbstract : public RendererObject
	{
	public:
		virtual VkRenderPass GetRenderPass() { return nullptr; }
	};

	class VreRenderPass : public VreRenderPassAbstract
	{
	public:
		VreRenderPass(VreRenderList* vreRenderList) { m_vreRenderList = vreRenderList; }

		void SetRenderToTexture(bool renderToTexture) { m_renderToTexture = renderToTexture; }

		void Create() override;
		void Destroy() override;

		VkRenderPass GetRenderPass() override;

	protected:
		VkRenderPass m_renderPass;
		VreRenderList* m_vreRenderList{};

		/// @brief Indicates whether the renderpass output will end in a swapchain image or in a texture
		/// and therefore affect the layout of the final image output.
		bool m_renderToTexture{ false };
	};
}