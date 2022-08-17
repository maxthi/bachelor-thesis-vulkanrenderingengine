#include "vre_drawer.hpp"
#include "../../vulkan_manager/vulkan_manager.hpp"
#include "vre_renderer.hpp"
#include "vre_commandbuffer.hpp"

namespace vre::Renderer::VreRenderer
{
	void VreDrawer::Create()
	{
		m_listOfRenderLists = VreRenderer::GetInstance()->GetRenderLists();
		auto it = m_listOfRenderLists->end();
		for (auto it = m_listOfRenderLists->rbegin(); it != m_listOfRenderLists->rend(); it++)
		{
			if ((*it)->IsMainScene())
			{
				m_mainScene = *it;
				break;
			}
		}

		m_swapchainSize = VulkanManager::GetInstance()->GetSwapchainInfo()->size;
		m_recordCommandBuffersTransparentObjects.resize(m_swapchainSize, false);
		m_recordCommandBuffersOpaqueObjects.resize(m_swapchainSize, false);

		m_imageAvailableSemaphores.resize(m_maximumFramesInFlight);
		m_renderFinishedSemaphores.resize(m_maximumFramesInFlight);
		m_fences.resize(m_maximumFramesInFlight);
		size_t swapchainSize = VulkanManager::GetInstance()->GetSwapchainInfo()->size;
		m_imagesInFlight.resize(swapchainSize, VK_NULL_HANDLE);

		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		VkDevice device = VulkanManager::GetInstance()->GetDevice();
		for (int32_t i = 0; i < m_maximumFramesInFlight; i++) {
			checkVk(
				VulkanInterface::vkCreateSemaphore(device, &semaphoreInfo, nullptr, &m_imageAvailableSemaphores[i]),
				"Renderer::Create: failed to create image available semaphore!"
			);

			checkVk(
				VulkanInterface::vkCreateSemaphore(device, &semaphoreInfo, nullptr, &m_renderFinishedSemaphores[i]),
				"Renderer::Create: failed to create render finished semaphore!"
			);

			checkVk(
				VulkanInterface::vkCreateFence(device, &fenceInfo, nullptr, &m_fences[i]),
				"Renderer::Create: failed to create render finished semaphore!"
			);
		}

		UpdateVulkanObjects();
	}

	void VreDrawer::Destroy()
	{
		VkDevice device = VulkanManager::GetInstance()->GetDevice();
		for (int32_t i = 0; i < m_maximumFramesInFlight; i++) {
			VulkanInterface::vkDestroySemaphore(device, m_renderFinishedSemaphores[i], nullptr);
			VulkanInterface::vkDestroySemaphore(device, m_imageAvailableSemaphores[i], nullptr);
			VulkanInterface::vkDestroyFence(device, m_fences[i], nullptr);

			m_renderFinishedSemaphores[i] = nullptr;
			m_imageAvailableSemaphores[i] = nullptr;
			m_fences[i] = nullptr;
		}
	}

	void VreDrawer::DrawFrame()
	{
		VulkanInterface::vkWaitForFences(m_device, 1, &m_fences[m_currentFrame], VK_TRUE, UINT64_MAX);

		uint32_t imageIndex;
		VkResult result = VulkanInterface::vkAcquireNextImageKHR(m_device, m_swapchain, UINT64_MAX, m_imageAvailableSemaphores[m_currentFrame], VK_NULL_HANDLE, &imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			recreateSwapchain();
			return;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			err("Renderer::DrawFrame: failed to acquire swap chain image!");
		}

		// check if this image returned by the swapchain is already in use for rendering
		if (m_imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
			VulkanInterface::vkWaitForFences(m_device, 1, &m_imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
		}
		// mark as in use for rendering
		m_imagesInFlight[imageIndex] = m_fences[m_currentFrame];

		updateUniformBuffers(imageIndex);

		// check if recording any command buffers has been requested from outside
		if (m_recordCommandBuffersTransparentObjects[imageIndex] || m_recordCommandBuffersOpaqueObjects[imageIndex])
		{
			for (auto it = m_listOfRenderLists->begin(); it != m_listOfRenderLists->end(); it++)
			{
                const auto& cmdBuffer = (*it)->GetVreCommandBuffer();
				if (cmdBuffer->IsRerecordingRequested(imageIndex))
				{
					cmdBuffer->RecordCommandBuffer(imageIndex, m_recordCommandBuffersTransparentObjects[imageIndex], m_recordCommandBuffersOpaqueObjects[imageIndex]);
				}
			}
			m_recordCommandBuffersTransparentObjects[imageIndex] = false;
			m_recordCommandBuffersOpaqueObjects[imageIndex] = false;
		}

		// submit command buffers to the graphics queue
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { m_imageAvailableSemaphores[m_currentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &(*m_commandBuffers)[imageIndex];

		// render finished semaphore, signaled by queue submit when rendering has finished and waited for in PresentImage
		VkSemaphore signalSemaphores[] = { m_renderFinishedSemaphores[m_currentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		VulkanInterface::vkResetFences(m_device, 1, &m_fences[m_currentFrame]);

		// m_fences is signaled when the command buffer finishes and a frame is finished and ready for presentation
		checkVk(
			VulkanInterface::vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, m_fences[m_currentFrame]),
			"VreRenderer::DrawFrame: failed to submit draw command buffer!"
		);

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { m_swapchain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;

		result = vkQueuePresentKHR(m_presentQueue, &presentInfo);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
			recreateSwapchain();
		}
		else if (result != VK_SUCCESS) {
			err("VreRenderer::DrawFrame: failed to present swap chain image!");
		}

		m_currentFrame = (m_currentFrame + 1) % m_maximumFramesInFlight;
	}

	void VreDrawer::UpdateVulkanObjects()
	{
		m_device = VulkanManager::GetInstance()->GetDevice();
		m_swapchain = VulkanManager::GetInstance()->GetSwapchainInfo()->swapchain;
		m_commandBuffers = m_mainScene.lock()->GetVreCommandBuffer()->GetCommandBuffers();
		m_graphicsQueue = VulkanManager::GetInstance()->GetLogicalDevice()->GetVkQueueGraphics();
		m_presentQueue = VulkanManager::GetInstance()->GetLogicalDevice()->GetVkQueuePresent();
	}

	void VreDrawer::TransparentDrawablesChanged()
	{
		for (size_t i = 0; i < m_recordCommandBuffersTransparentObjects.size(); i++)
		{
			m_recordCommandBuffersTransparentObjects[i] = true;
		}
	}

	void VreDrawer::OpaqueDrawablesChanged()
	{
		for (size_t i = 0; i < m_recordCommandBuffersOpaqueObjects.size(); i++)
		{
			m_recordCommandBuffersOpaqueObjects[i] = true;
		}
	}

	void VreDrawer::updateUniformBuffers(uint32_t imageIndex)
	{
		for (auto& renderList : *m_listOfRenderLists)
		{
			for (const auto& bufferList : *renderList->GetOpaqueDrawables())
			{
				for (const auto& drawableData : bufferList.mDrawableDataList)
				{
					drawableData->mUniformBuffer->UpdateUniformBuffer(imageIndex);
				}
			}

			for (const auto& bufferList : *renderList->GetTransparentDrawables())
			{
				for (const auto& drawableData : bufferList.mDrawableDataList)
				{
					drawableData->mUniformBuffer->UpdateUniformBuffer(imageIndex);
				}
			}
		}
	}

	void VreDrawer::recreateUniformBuffers()
	{
		for (auto& renderList : *m_listOfRenderLists)
		{
			for (const auto& bufferList : *renderList->GetOpaqueDrawables())
			{
				for (const auto& drawableData : bufferList.mDrawableDataList)
				{
					drawableData->mUniformBuffer->Destroy();
					drawableData->mUniformBuffer->Create(drawableData);
				}
			}

			for (const auto& bufferList : *renderList->GetTransparentDrawables())
			{
				for (const auto& drawableData : bufferList.mDrawableDataList)
				{
					drawableData->mUniformBuffer->Destroy();
					drawableData->mUniformBuffer->Create(drawableData);
				}
			}
		}
	}

	void VreDrawer::recreateSwapchain()
	{
		// only recreate swapchain if window isn't minimized
		auto swapchainDetails = VulkanManager::GetInstance()->GetPhysicalDevice()->GetSwapchainSupportDetails();
		if (swapchainDetails->capabilities.maxImageExtent.width == 0 || swapchainDetails->capabilities.maxImageExtent.height == 0)
		{
			// window is minimized. skip swapchain recreation
			return;
		}

		VulkanManager::GetInstance()->RecreateSwapchain();
		UpdateVulkanObjects();
		size_t swapchainSize = VulkanManager::GetInstance()->GetSwapchainInfo()->size;
		if (m_swapchainSize != swapchainSize)
		{
			// swapchain size changed, new number of uniform buffers needed
			recreateUniformBuffers();
			m_recordCommandBuffersTransparentObjects.resize(m_swapchainSize, false);
			m_recordCommandBuffersOpaqueObjects.resize(m_swapchainSize, false);
		}
	}
}
