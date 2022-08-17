#include "vre_renderlist.hpp"

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
	VreRenderList::VreRenderList() :
		m_isMainScene(true)
	{
		// get swapchain properties and use them as render to to
		auto swapchainInfo = VulkanManager::GetInstance()->GetSwapchainInfo();
		m_renderTargetInfo.mWidth = swapchainInfo->extent2D.width;
		m_renderTargetInfo.mHeight = swapchainInfo->extent2D.height;
		m_renderTargetInfo.mFormat = swapchainInfo->format;
		m_renderTargetInfo.mImageViews = swapchainInfo->pImageViews;
	}

	VreRenderList::VreRenderList(uint32_t width, uint32_t height, std::vector<VkImageView>* renderTargetImageViews, VkFormat format, float clearColorValues[4])
	{
		m_renderTargetInfo.mWidth = width;
		m_renderTargetInfo.mHeight = height;
		m_renderTargetInfo.mFormat = format;
		m_renderTargetInfo.mImageViews = renderTargetImageViews;
		for (int i = 0; i < 4; i++)
		{
			m_renderTargetInfo.mClearValuesColor[i] = clearColorValues[i];
		}
	}

	void VreRenderList::Initialize()
	{
		// create attachments
		if (m_vreAttachments == nullptr)
			m_vreAttachments = std::make_shared<VreAttachments>(this);
		// create renderpass
		if (m_vreRenderPass == nullptr)
		{
			auto renderpass = std::make_shared<VreRenderPass>(this);
			if (!m_isMainScene)
			{
				renderpass->SetRenderToTexture(true);
			}
			m_vreRenderPass = renderpass;
		}
		// create framebuffers
		if (m_vreFrameBuffer == nullptr)
			m_vreFrameBuffer = std::make_shared<VreFrameBuffer>(this);
		// create pipeline
		if (m_vreGraphicsPipelines == nullptr)
			m_vreGraphicsPipelines = std::make_shared<VreGraphicsPipelines>(this);
		// create command buffers
		if (m_vreCommandBuffer == nullptr)
			m_vreCommandBuffer = std::make_shared<VreCommandBuffer>(this);

		createSwapchainDependentObjects();

		// if this render list uses the swapchain images as render target
		// the vulkan objects need recreation when the swapchain changes
		// dimensions or number of images
		// otherwise no swapchain recreation callback is added.
		if (m_isMainScene)
		{
			VulkanManager::GetInstance()->AddSwapchainRecreationCallback(
				[this](bool swapchainSizeChanged)
				{
					swapchainRecreationCallback(swapchainSizeChanged);
				}
			);
		}
	}

	void VreRenderList::Destroy()
	{
		destroySwapchainDependentObjects();
	}

	void VreRenderList::ClearPipelineSets()
	{
		m_pipelineSets.clear();
	}

	void VreRenderList::AddPipelineSet(uint64_t shaderCombinationIndex, std::shared_ptr<VrePipelineSet> pipelineSet)
	{
		m_pipelineSets.insert({shaderCombinationIndex, pipelineSet});
	}

	void VreRenderList::createSwapchainDependentObjects()
	{
		m_vreAttachments->Create();
		m_vreRenderPass->Create();
		m_vreFrameBuffer->Create();
		m_vreGraphicsPipelines->Create();
		m_vreCommandBuffer->Create();
	}

	void VreRenderList::destroySwapchainDependentObjects()
	{
		m_vreCommandBuffer->Destroy();
		m_vreGraphicsPipelines->Destroy();
		m_vreFrameBuffer->Destroy();
		m_vreRenderPass->Destroy();
		m_vreAttachments->Destroy();
	}

	void VreRenderList::swapchainRecreationCallback(bool swapchainSizeChanged)
	{
		// update properties according to swapchain
		auto swapchainInfo = VulkanManager::GetInstance()->GetSwapchainInfo();
		m_renderTargetInfo.mWidth = (int)swapchainInfo->extent2D.width;
		m_renderTargetInfo.mHeight = (int)swapchainInfo->extent2D.height;
		m_renderTargetInfo.mFormat = swapchainInfo->format;
		m_renderTargetInfo.mImageViews = swapchainInfo->pImageViews;

		// swapchainSize is irrelevant because swapchain dimensions matter
		destroySwapchainDependentObjects();
		createSwapchainDependentObjects();
	}
}