#pragma once
#include "../vulkan_manager/image.hpp"
#include "../common.hpp"
#include "renderer_object.hpp"

namespace vre::Renderer
{
	/// @brief Renderer is an abstract class for renderer implementations.
	/// A renderer subclass can be passed to the VulkanManager. The VulkanManager will invoke the methods, such as DrawFrame.
	/// The renderers purpose is to read the scenegraph and setup and the vulkan renderer functionality, such as:
	/// - the renderpass
	/// - graphic pipelines
	/// - framebuffers & attachements
	/// - commandbuffers & commandpool
	class Renderer
	{
	public:
		/// @brief Called by the vulkan manager when its time to initialize the renderer.
		virtual void Initialize() = 0;

		/// @brief Called when the application is about to close and all objects need to be freed.
		virtual void Destroy() = 0;

		/// @brief Call to the renderer to draw a frame.
		virtual void DrawFrame() = 0;

	protected:
		Renderer() {};
		virtual ~Renderer() {};
	};
}