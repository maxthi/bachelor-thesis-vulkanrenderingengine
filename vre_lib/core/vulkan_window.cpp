#include "vulkan_window.hpp"
#include <QEvent>
#include "../scene_manager/scene_manager.hpp"
#include "../shader_manager/shader_manager.hpp"
#include "../vulkan_renderer/vre_renderer/vre_renderer.hpp"

namespace vre {
    VulkanWindow::VulkanWindow() :
        m_initialized(false)
    {
        this->setSurfaceType(QSurface::VulkanSurface);
    }

    VulkanWindow::~VulkanWindow()
    {
        // make sure to cleanup all resources if the render view is destroyed
        m_vreCorePtr->Cleanup();

        // expected to have no more calls to update render but still have the qt app
        VulkanManager::GetInstance()->DestroyAfterSurfaceDestroyed();
    }

    bool VulkanWindow::event(QEvent* event)
    {
        switch (event->type()) {
        case QEvent::KeyPress:
        case QEvent::KeyRelease:
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonRelease:
        case QEvent::MouseMove:
        case QEvent::MouseButtonDblClick:
            m_vreCorePtr->HandleEvent(event);
            return true;
        case QEvent::UpdateRequest:
            render();
            return true;
        case QEvent::PlatformSurface:
            // according to https://code.woboq.org/qt5/qtbase/src/gui/vulkan/qvulkanwindow.cpp.html
            // The swapchain must be destroyed before the surface as per spec. This is
            // not ideal for us because the surface is managed by the QPlatformWindow
            // which may be gone already when the unexpose comes, making the validation
            // layer scream. The solution is to listen to the PlatformSurface events.
            if (static_cast<QPlatformSurfaceEvent*>(event)->surfaceEventType() == QPlatformSurfaceEvent::SurfaceAboutToBeDestroyed) {
                m_vreCorePtr->SurfaceAboutToBeDestroyed();
            }
            return true;
        default:
            return QWindow::event(event);
        }
    }

    void VulkanWindow::exposeEvent(QExposeEvent*)
    {
        if (isExposed()) {
            if (!m_initialized)
            {
                m_vreCorePtr = VreCore::GetInstance();
                m_vreCorePtr->Setup(this);
                m_initialized = true;
                // draw first frame
                render();
            }
        }
    }

    void VulkanWindow::render()
    {
        if(m_initialized)
            m_vreCorePtr->Update();

        // continuous rendering
        requestUpdate();
    }
}

