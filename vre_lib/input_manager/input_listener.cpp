#include "input_listener.hpp"

namespace vre
{
	EventDispatching InputListener::KeyPressed(QEvent* event)
	{
		return EventDispatching::PassEvent;
	}

	EventDispatching InputListener::KeyReleased(QEvent* event)
	{
		return EventDispatching::PassEvent;
	}

	EventDispatching InputListener::MousePressed(QEvent* event)
	{
		return EventDispatching::PassEvent;
	}

	EventDispatching InputListener::MouseDblClick(QEvent* event)
	{
		return EventDispatching::PassEvent;
	}

	EventDispatching InputListener::MouseReleased(QEvent* event)
	{
		return EventDispatching::PassEvent;
	}

	EventDispatching InputListener::MouseMoved(QEvent* event)
	{
		return EventDispatching::PassEvent;
	}
}