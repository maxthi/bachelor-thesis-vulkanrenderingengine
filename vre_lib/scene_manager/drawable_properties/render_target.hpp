#pragma once
#include "drawable_property.hpp"
#include "../../vulkan_manager/image.hpp"
#include "../../common.hpp"
#include "../scene.hpp"

namespace vre
{
	struct DrawableData;

	/// @brief A RenderTargetConfiguration object contains all the data needed by a
	/// RenderTarget property.
	struct RenderTargetConfiguration
	{
		/// @brief The root node of this render target.
		std::weak_ptr<Node> mRootNode{};

		/// @brief The height of the render target texture.
		uint32_t mHeight{ 0 };

		/// @brief The width of the render target texture.
		uint32_t mWidth{ 0 };

		/// @brief Color format of the render target texture.
		VkFormat mFormat{ VK_FORMAT_B8G8R8A8_SRGB };

		float mClearColorValues[4] = {0.0f, 0.0f, 0.0f, 1.0f};

		/// @brief Camera holding camera info.
		std::shared_ptr<Camera> mCamera;

		/// @brief The rendertargets name, can be used for debugging.
		std::string mName{ "UnnamedRenderTargetConfiguration" };
	};

	/// @brief Similar to a 2DTexture a RenderTarget is a vulkan buffer that can be sampled from in a shader.
	/// The difference is that a RenderTarget contains a root node to a scenegraph, which is then rendered into a
	/// texture.
	class RenderTarget : public DrawableProperty
	{
	public:
		static DrawablePropertyId GetClassPropertyId() {
			static DrawablePropertyId propertyId = DrawableProperty::GetUniquePropertyId();
			return propertyId;
		}

		/// @brief Allocate images on the GPU that can be rendered to.
		/// @param config - Configuration of the allocated texture.
		RenderTarget(std::shared_ptr<RenderTargetConfiguration> config);

		~RenderTarget();

		/// @brief Get the configuration used for this render target.
		std::shared_ptr<RenderTargetConfiguration> GetConfiguration() { return m_config; }

		/// @brief Get the image views to the allocated images.
		std::vector<VkImageView>* GetImageViews() { return &m_imageViews; }

		//TODO: needs recreation when swapchain size changes, because there is one render target for each swapchain img.
		/// @brief Creating the rendertargets.
		void AllocateMemory(std::shared_ptr<DrawableData>& drawableData) override;

	private:
		/// @brief The config defining the render target textures.
		std::shared_ptr<RenderTargetConfiguration> m_config;

		/// @brief The image buffers on the gpu.
		std::vector<std::shared_ptr<vre::Image>> m_textures;

		/// @brief The image views according to the allocated textured.
		std::vector<VkImageView> m_imageViews;
	};
}