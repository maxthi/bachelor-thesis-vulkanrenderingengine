#pragma once
#include <QWindow>
#include <qmainwindow.h>
#include "vre_core.hpp"

namespace vre
{
    /// @brief The vulkan window inherits directly from QWindow and provides a render surface with qts basic functionality.
    /// It does not provide Qt Widget support. If you want to use Qt widgets, use RunVreGUI instead of RunVre.
    class VulkanWindow : public QWindow
    {
    public:
        /// @brief The constructor of the vulkan window tells qt that we are using vulkan as render surface.
        VulkanWindow();

        /// @brief Upon window destruction this will automaticall call
        ~VulkanWindow();

    protected:
        /// @brief Handles all incoming qt events.
        /// @param event - The occuring qt event to be handled.
        bool event(QEvent* event) override;

        /// @brief Used to initialise the vre core once vulkan window has finished its setup.
        void exposeEvent(QExposeEvent*) override;

        /// @brief Invokes the draw call of the vre and afterwards calls RequestUpdate() which will
        /// trigger the update event in the qt event queue as soon as available.
        void render();

    private:
        /// @brief Pointer to the static vre core instance.
        VreCore* m_vreCorePtr;

        /// @brief Set to true by expose event when the qt window has finished initialisation and
        /// therefore vre core has been initialised as well.
        bool m_initialized;
    };
}
