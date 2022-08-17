#include "../../core/vre_core.hpp"
#include "vre_renderer.hpp"
#include "vre_attachments.hpp"
#include "vre_renderpass.hpp"
#include "../renderer_object.hpp"
#include "vre_graphicspipelines.hpp"
#include "vre_commandbuffer.hpp"
#include "vre_framebuffer.hpp"
#include "vre_commandpool.hpp"
#include "vre_drawer.hpp"

namespace vre::Renderer::VreRenderer
{
	void VreRenderer::Initialize()
	{
		// create command pool
		if (m_vreCommandPool == nullptr)
			m_vreCommandPool = std::make_shared<VreCommandPool>();

		if (m_vreDrawer == nullptr)
			m_vreDrawer = std::make_shared<VreDrawer>();

		m_vreCommandPool->Create();

		for (auto& renderList : *m_listOfRenderLists)
		{
			renderList->Initialize();
		}

		m_vreDrawer->Create();
	}

	void VreRenderer::Destroy()
	{
		VkDevice device = VulkanManager::GetInstance()->GetDevice();
		VulkanInterface::vkDeviceWaitIdle(device);
		m_vreDrawer->Destroy();
		m_vreDrawer.reset();

		for (auto& renderList : *m_listOfRenderLists)
		{
			renderList->Destroy();
		}

		m_listOfRenderLists.reset();

		m_vreCommandPool->Destroy();
		m_vreCommandPool.reset();
		m_destroyed = true;
	}

	void VreRenderer::DrawFrame()
	{
		m_vreDrawer->DrawFrame();
	}

	void VreRenderer::TransparentDrawablesChanged()
	{
		std::dynamic_pointer_cast<VreDrawer>(m_vreDrawer)->TransparentDrawablesChanged();
	}

	void VreRenderer::OpaqueDrawablesChanged()
	{
		std::dynamic_pointer_cast<VreDrawer>(m_vreDrawer)->OpaqueDrawablesChanged();
	}

	VreRenderer* VreRenderer::GetInstance()
	{
		static VreRenderer* rendererInstance = new VreRenderer();
		return rendererInstance;
	}

	void VreRenderer::RecordCommandBuffer(uint32_t imageIndex, bool recordTransparent, bool recordOpaque)
	{
	   // event driven -> move camera, check if oject order changes -> if yes, rerecord primary
	   // changed scene -> update secondary.

		// for all render lists, rerecord if camera moved..
		for (auto& renderlist : *m_listOfRenderLists)
		{
			renderlist->GetVreCommandBuffer()->RecordCommandBuffer(imageIndex, recordTransparent, recordOpaque);
		}

	}

	const std::vector<VkCommandBuffer>* VreRenderer::GetCommandBuffers()
	{
		return nullptr;
	}

	VreRenderer::~VreRenderer()
	{
		// delete renderer objects in reverse creation order
		if (!m_destroyed)
		{
			Destroy();
		}
	}
}
