#pragma once
#include "../common.hpp"
#include <QEvent>
#include <list>
#include "input_listener.hpp"

namespace vre
{
	/// @brief The input manager will receive all events created by the qt window and pass them to the registered listeners.
	/// The first registered listener will receive the event at first and then decide whether to catch the event or to pass it
	/// to the next registered listener.
	class InputManager
	{
	public:
		/// @brief The input manager is available as a static instance.
		static InputManager* GetInstance() { static InputManager ipm; return &ipm; }

		/// @brief Handle event is called by the qt window to pass its events to the handler.
		void HandleEvent(QEvent* event);

		/// @brief Registers a listener to catch events. Default is to the front, so a newly registered listener receives events first.
		void RegisterListener(std::shared_ptr<InputListener> listener, bool toFront = true);

	private:
		/// @brief The list of all registered listeners.
		std::list<std::shared_ptr<InputListener>> m_listeners;
	};
}
