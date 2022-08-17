#pragma once
#include "../common.hpp"
#include "../vulkan_manager/vulkan_manager.hpp"
#include "../input_manager/input_manager.hpp"
#include "../scene_reader/scene_reader.hpp"
#include "../scene_manager/scene_manager.hpp"
#include "../vulkan_renderer/vre_renderer/vre_renderer.hpp"

namespace vre
{
	/// @brief The project class controls the program flow after the qt window setup has finished and is ready to render. 
	/// Its an intended mix of methods that are defined by the framework to control the mainflow
	/// and methods to be defined by the user in order to setup the scene, allocate scene specific memory and freeing it afterwards.
	/// However the user may overwrite all methods of the project class to obtain complete control over the program flow,
	/// like when to create the vulkan manager, the renderer etc.
	class Project
	{
	public:

		/// @brief Method for the project to allocate memory on the GPU and feed the scenemanager with a scene.
		virtual void Setup(std::shared_ptr<SceneManager> sceneManager) = 0;

		/// @brief Method intended to free all ressources created by Setup.
		virtual void Cleanup() {};

		/// @brief Unless overwritten called by CompleteUpdate whenever the qt window updates the render view for a new frame.
		virtual void Update() {};

		/// @brief This method controls the main flow.
		/// Usual order:
		/// - setup scenes.
		/// - read active scene.
		/// It can be overwritten to gain better control. In that case, CompleteCleanup should also be overwritten, as it clears
		/// the default ressources allocated in default CompleteSetup.
		/// @param window - The window handle that has to be passed to the VulkanManager.
		virtual void CompleteSetup(QWindow* window);

		/// @brief Allows to manage the deallocation process completely, just like CompleteSetup allows to manage the allocate process.
		/// It makes sense to overwrite CompleteCleanup when overwriting Execute.
		virtual void CompleteCleanup();

		/// @brief Called each time the qt window is ready to update, aka render the next frame.
		virtual void CompleteUpdate();

		/// @brief 
		virtual EventDispatching KeyPressed(Qt::Key key) { return EventDispatching::PassEvent; };

	protected:
		/// @brief Unless CompleteSetup is overwritten, this value holds the pointer to the renderer in use.
		Renderer::VreRenderer::VreRenderer* m_renderer{};
	};
}