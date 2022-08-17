#pragma once
#include <qapplication.h>
#include <qevent.h>
#include "../common.hpp"
#include "input_listener.hpp"
#include "../core/project.hpp"

namespace vre
{
	/// @brief The vre main input listener.
	class VreInputListener : public InputListener
	{
	public:
		VreInputListener(QWindow* window, Project* project) : m_window(window), m_project(project) {}
		EventDispatching KeyPressed(QEvent* event);
		EventDispatching KeyReleased(QEvent* event);
		EventDispatching MousePressed(QEvent* event);
		EventDispatching MouseReleased(QEvent* event);
		EventDispatching MouseDblClick(QEvent* event);
		EventDispatching MouseMoved(QEvent* event);

	private:
		/// @brief A handle to the qt window sending the events.
		QWindow* m_window{};

		/// @brief Reference to the vre project for quick key access. If more advance input events are required,
		/// register a new input handler.
		Project* m_project{};

		bool m_mouseControlsCamera{ false };
	};
}
