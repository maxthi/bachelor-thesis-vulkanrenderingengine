#pragma once
#include "../common.hpp"
#include "../vulkan_manager/vulkan_configuration.hpp"
#include "project.hpp"
#include <qmainwindow.h>

namespace vre
{
	/// @brief Contains certain configuration options that can be set before running the vre.
	struct VreConfiguration
	{
		/// @brief Everything directly concerning vulkan.
		VulkanConfiguration mVulkanCfg{};

		/// @brief The window title and application name.
		std::string mApplicationName{ "Unnamed" };

		/// @brief The width of the window created.
		uint32_t mWindowWidth{ 1024 };

		/// @brief The height of the window created.
		uint32_t mWindowHeight{ 768 };

		/// @brief The project to be executed. If it remains set to nullptr, the application will terminate.
		std::shared_ptr<Project> mProject{ nullptr };

		/// @brief Will output all Qt/Vulkan output to a logfile.
		bool mCreateLogfile{ true };

		/// @brief This is set to true by the RunVulkanRenderEngineWithGUI method if the rendering window
		/// is used as a widget in another GUI class.
		bool mIsEmbeddedInGUI{ false };

		/// @brief If a window is hosting the VulkanWindow, this pointer points to the hosting main window.
		QMainWindow* mHostWindow{ nullptr };

		/// @brief Holds the path where the applications resource files are located. This can be set in a project
		///  by defining and using vre::Filesystem::getResourcePath.
		///  If left to "" the application assumes your resources are located next to the executable.
		std::string mPathToResources{ "" };
	};
}