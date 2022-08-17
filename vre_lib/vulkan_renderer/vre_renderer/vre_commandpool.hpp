#pragma once
#include "vre_renderer.hpp"

namespace vre::Renderer::VreRenderer
{
	class VreCommandPoolAbstract : public RendererObject
	{
	public:
		virtual VkCommandPool GetCommandPool() { return nullptr; }
	};

	class VreCommandPool : public VreCommandPoolAbstract
	{
	public:
		void Create() override;
		void Destroy() override;

		VkCommandPool GetCommandPool() override { return m_commandPool; }

	private:
		VkCommandPool m_commandPool{};
	};
}