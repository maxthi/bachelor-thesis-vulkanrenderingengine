#pragma once
#include <qapplication.h>
#include <qevent.h>
#include "../common.hpp"

namespace vre
{
	/// @brief Defines how the method that returns has handled the event. Whether it has catched the event,
	/// meaning it has acted on the event and no more further action should be taken or
	/// the event shall be passed, which means other event handlers can still (additionally) act upon the event.
	enum class EventDispatching
	{
		CatchEvent,
		PassEvent
	};

	/// @brief The input listener is the base class to inherit any input handler from. It will pass any event by default.
	/// Overwrite the desired events callbacks to catch the events.
	class InputListener
	{
	public:
		virtual EventDispatching KeyPressed(QEvent* event);
		virtual EventDispatching KeyReleased(QEvent* event);
		virtual EventDispatching MousePressed(QEvent* event);
		virtual EventDispatching MouseReleased(QEvent* event);
		virtual EventDispatching MouseDblClick(QEvent* event);
		virtual EventDispatching MouseMoved(QEvent* event);
	};
}
