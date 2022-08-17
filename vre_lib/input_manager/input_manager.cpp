#include "input_manager.hpp"

namespace vre
{
	void InputManager::HandleEvent(QEvent* event)
	{
		// populates the event to all registered listeners
		// from front to back, each listener can decide to pass an event or to catch it
		if (m_listeners.size() == 0)
		{
			return;
		}
		switch (event->type())
		{
		case QEvent::KeyPress:
			for (const auto pListener : m_listeners) {
				if (pListener->KeyPressed(event) == EventDispatching::CatchEvent) {
					break;
				}
			}
			break;
		case QEvent::KeyRelease:
			for (const auto pListener : m_listeners) {
				if (pListener->KeyReleased(event) == EventDispatching::CatchEvent) {
					break;
				}
			}
			break;
		case QEvent::MouseButtonPress:
			for (const auto pListener : m_listeners) {
				if (pListener->MousePressed(event) == EventDispatching::CatchEvent) {
					break;
				}
			}
			break;
		case QEvent::MouseButtonRelease:
			for (const auto pListener : m_listeners) {
				if (pListener->MouseReleased(event) == EventDispatching::CatchEvent) {
					break;
				}
			}
			break;
		case QEvent::MouseButtonDblClick:
			for (const auto pListener : m_listeners) {
				if (pListener->MouseDblClick(event) == EventDispatching::CatchEvent) {
					break;
				}
			}
			break;
		case QEvent::MouseMove:
			for (const auto pListener : m_listeners) {
				if (pListener->MouseMoved(event) == EventDispatching::CatchEvent) {
					break;
				}
			}
			break;
		default:
			err("InputManager::HandleEvent: Unhandled event occured!");
			return;
		}

	}

	void InputManager::RegisterListener(std::shared_ptr<InputListener> listener, bool toFront)
	{
		if (toFront) {
			m_listeners.push_front(listener);
		}
		else {
			m_listeners.push_back(listener);
		}
	}
}
