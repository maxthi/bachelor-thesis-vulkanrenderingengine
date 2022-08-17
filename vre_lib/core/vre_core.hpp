#pragma once
#include "../common.hpp"
#include "../vulkan_manager/vulkan_manager.hpp"
#include "../input_manager/input_manager.hpp"
#include "../scene_reader/scene_reader.hpp"
#include "../scene_manager/scene_manager.hpp"
#include "project.hpp"
#include "vre_configuration.hpp"
#include <QMainWindow>

namespace vre
{
	class VulkanWindow;

	/// @brief The VreCore class connects the qt window events with the vre, like updating
	/// it can be accessed any time. Therefore it can be used the store the configuration prior to setting up
	class VreCore
	{
	public:
		static VreCore* GetInstance() { static VreCore core; return &core; }

		void SetConfig(VreConfiguration* vreConfig) { m_vreConfig = vreConfig; }
		VreConfiguration* GetConfig() { return m_vreConfig; }

		void Setup(QWindow* window);
		void Update();
		void HandleEvent(QEvent* event);
		void SurfaceAboutToBeDestroyed();

		/// @brief Can be called from anywhere and will destroy the window, which will result in a termination of the program.
		void Quit();

		/// @brief Called upon window destruction and calls the cleanup methods of the project.
		void Cleanup();

		/// @brief Returns the main window embedding the vulkan render view.
		/// Works only if there is a GUI used embedding the render view.
		/// @return A pointer to the MainWindow.
        QMainWindow* GetHostWindow();

	private:
		/// @brief The project that contains the workflow of the vre.
		std::shared_ptr<Project> m_project;

		/// @brief A pointer to the config that can be passed by a project to configure the setup.
		VreConfiguration* m_vreConfig{ nullptr };

		/// @brief A handle to the window created by qt.
		QWindow* m_window{ nullptr };

		/// @brief Set to true when Quit() is called. Make sure cleanup is executed only once.
		bool m_hasQuit{ false };

		/// @brief The default constructor is private because the only instance is static and can be
		/// obtained by a call to GetInstance.
		VreCore() = default;

		/// @brief A strong reference to the scene manager to keep it alive during runtime.
		std::shared_ptr<SceneManager> m_sceneManager;
	};
}
