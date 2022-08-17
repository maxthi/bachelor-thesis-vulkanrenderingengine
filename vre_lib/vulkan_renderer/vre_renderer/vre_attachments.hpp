#pragma once
#include "vre_renderer.hpp"
#include "../../vulkan_manager/image.hpp"

namespace vre::Renderer::VreRenderer
{
	/// @brief The abstract class can be used to exchange the attachments object in the renderer and
	// implement a custom depth/msaa image.
	class VreAttachmentsAbstract : public RendererObject
	{
	public:
		virtual std::shared_ptr<Image> GetDepthImage() { return nullptr; }
		virtual std::shared_ptr<Image> GetMSAAImage() { return nullptr; }
	};

	class VreRenderList;

	/// @brief This class contains framebuffer attachments that are utilized during frame rendering by the pipeline.
	/// For the vre renderer the used attachments are a depth image and an msaa image.
	class VreAttachments : public VreAttachmentsAbstract
	{
	public:
		VreAttachments(VreRenderList* vreRenderList) { m_vreRenderList = vreRenderList; }

		void Create() override;
		void Destroy() override;

		std::shared_ptr<Image> GetDepthImage() override;
		std::shared_ptr<Image> GetMSAAImage() override;

	protected:
		VreRenderList* m_vreRenderList{};
		std::shared_ptr<Image> m_depthImage;
		std::shared_ptr<Image> m_msaaImage;

		void createDepthBuffer();
		void destroyDepthBuffer();
		VkFormat findDepthFormat();

		void createMSAABuffer();
		void destroyMSAABuffer();

	};
}