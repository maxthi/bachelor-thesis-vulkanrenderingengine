#include "vre_core.hpp"
#include "../scene_manager/scene_manager.hpp"
#include "../scene_reader/scene_reader.hpp"
#include "../shader_manager/shader_manager.hpp"
#include "../vulkan_renderer/vre_renderer/vre_renderer.hpp"
#include "../input_manager/vre_input_listener.hpp"
#include <qcursor.h>
#include <QLoggingCategory>

namespace vre
{
    void VreCore::Setup(QWindow* window)
    {
        m_window = window;

        if(m_vreConfig->mProject == nullptr)
        {
            err("VreCore: No project has been set, therfore the application terminates. Make sure to set a project in the passed VreConfiguration.");
            Quit();
            return;
        }

        if (m_vreConfig->mCreateLogfile)
        {
            Logging::ActivateLogging();
        }

        auto vulkanManager = VulkanManager::GetInstance();
        vulkanManager->SetQWindow(window);
        vulkanManager->Create();

        m_sceneManager = SceneManager::GetInstance();

        Logging::Timer timer;
        m_vreConfig->mProject->CompleteSetup(window);
        log("Info: Project setup took " + std::to_string(timer.Milliseconds()) + " Milliseconds.");
    }

    void VreCore::Update()
    {
        m_vreConfig->mProject->CompleteUpdate();
    }

    void VreCore::HandleEvent(QEvent* event)
    {
        InputManager::GetInstance()->HandleEvent(event);
    }

    void VreCore::SurfaceAboutToBeDestroyed()
    {
        VulkanManager::GetInstance()->DestroyBeforeSurfaceDestroyed();
    }

    void VreCore::Quit()
    {
        if (!m_hasQuit)
        {
            m_hasQuit = true;
            m_window->close(); // close doesn't affect an embedded widget
            m_window->destroy(); // destroy removes the render view used as widget
        }
        else
        {
            log("Warning: VreCore::Quit called twice.");
        }
    }

    void VreCore::Cleanup()
    {
        m_vreConfig->mProject->CompleteCleanup();
        m_sceneManager.reset();
    }

    QMainWindow* VreCore::GetHostWindow()
    {
        if (!m_vreConfig->mIsEmbeddedInGUI)
        {
            err("VreCore::GetHostWindow: This render");
        }
        if (m_vreConfig->mHostWindow == nullptr)
        {
            err("VreCore::GetHostWindow: MainWindow is a nullptr.");
        }

        return m_vreConfig->mHostWindow;
    }
}
