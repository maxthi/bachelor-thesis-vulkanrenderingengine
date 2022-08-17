/// \mainpage Vulkan Rendering EngineX
/// blabla
///

#pragma once
#include "vre_configuration.hpp"
#include "vre_core.hpp"
#include "vulkan_window.hpp"

namespace vre
{
    /// @brief Running the vre in a raw QWindow inherited window, using the complete background as rendering surface.
    /// @param argc - Number of call arguments.
    /// @param argv - Call arguments.
    /// @param vreConfig - Configuration of the vre.
    /// @return - The exit code of the Qt window.
    int RunVre(int argc, char* argv[], VreConfiguration& vreConfig);

    /// @brief Embeds the vre as central widget of a QMainWindow. Can be used to simply add dock widgets.
    /// @param argc - Number of call arguments.
    /// @param argv - Call arguments.
    /// @param vreConfig - Configuration of the vre.
    /// @return - The exit code of the Qt window.
    int RunVreGUI(int argc, char* argv[], VreConfiguration& vreConfig);

    /// @brief Same as RunVreGUI but with a template so a custom window class can be provided.
    /// The template class therefore must be inherited from QMainWindow.
    /// @tparam windowClass - The window class to use as main window. MUST be inherited from QMainWindow.
    /// @param argc - Number of call arguments.
    /// @param argv - Call arguments.
    /// @param vreConfig - Configuration of the vre.
    /// @param setCentralWidget - If true, sets the VulkanWindow as central widget of the QMainWindow.
    /// @return - The exit code of the Qt window.
    template<typename windowClass>
    int RunVreGUI(int argc, char* argv[], VreConfiguration& vreConfig, bool setCentralWidget = true)
    {
        QApplication a(argc, argv);
        windowClass w;
        w.show();

        vreConfig.mIsEmbeddedInGUI = true;
        vreConfig.mHostWindow = &w;
        VreCore::GetInstance()->SetConfig(&vreConfig);

        if (setCentralWidget)
        {
            vre::VulkanWindow* window = new vre::VulkanWindow();
            w.setCentralWidget(w.createWindowContainer(window, &w));
        }

        w.setWindowTitle(vreConfig.mApplicationName.c_str());
        w.resize(vreConfig.mWindowWidth, vreConfig.mWindowHeight);
        w.show();
        return a.exec();
    }
}
