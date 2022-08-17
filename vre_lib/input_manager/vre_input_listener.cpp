#include "vre_input_listener.hpp"
#include "../scene_manager/scene_manager.hpp"
#include <qwindow.h>
#include "../core/vre_core.hpp"

namespace vre
{
	EventDispatching VreInputListener::KeyPressed(QEvent* event)
	{
		auto camera = SceneManager::GetInstance()->GetActiveScene()->GetCamera();
		QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

		// pass key to project first
		if (m_project->KeyPressed((Qt::Key)keyEvent->key()) == EventDispatching::CatchEvent)
		{
			return EventDispatching::CatchEvent;
		}

		switch (keyEvent->key())
		{
		case Qt::Key::Key_Up:
			camera->SetMoveForwards(true);
			return EventDispatching::CatchEvent;
		case Qt::Key::Key_Down:
			camera->SetMoveBackwards(true);
			return EventDispatching::CatchEvent;
		case Qt::Key::Key_Left:
			camera->SetMoveLeft(true);
			return EventDispatching::CatchEvent;
		case Qt::Key::Key_Right:
			camera->SetMoveRight(true);
			return EventDispatching::CatchEvent;
		case Qt::Key::Key_Escape:
			VreCore::GetInstance()->Quit();
			break;
		case Qt::Key::Key_Space:
			auto camEye = camera->GetCameraEye();
			auto camCenter = camera->GetCameraFront();
			auto eyeStr = std::string("CameraEye:") + std::to_string(camEye->x()) + std::string(" ") + std::to_string(camEye->y()) + std::string(" ") + std::to_string(camEye->z());
			auto centerStr = std::string("CameraFront:") + std::to_string(camCenter->x()) + std::string(" ") + std::to_string(camCenter->y()) + std::string(" ") + std::to_string(camCenter->z());
			log(eyeStr + centerStr);
			break;
		}
		return EventDispatching::PassEvent;
	}

	EventDispatching VreInputListener::KeyReleased(QEvent* event)
	{
		auto camera = SceneManager::GetInstance()->GetActiveScene()->GetCamera();
		QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
		switch (keyEvent->key())
		{
		case Qt::Key::Key_Up:
			camera->SetMoveForwards(false);
			return EventDispatching::CatchEvent;
		case Qt::Key::Key_Down:
			camera->SetMoveBackwards(false);
			return EventDispatching::CatchEvent;
		case Qt::Key::Key_Left:
			camera->SetMoveLeft(false);
			return EventDispatching::CatchEvent;
		case Qt::Key::Key_Right:
			camera->SetMoveRight(false);
			return EventDispatching::CatchEvent;
		}
		return EventDispatching::PassEvent;
	}

	EventDispatching VreInputListener::MousePressed(QEvent* event)
	{
		QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
		if (mouseEvent->button() == Qt::MouseButton::RightButton)
		{
			m_mouseControlsCamera = !m_mouseControlsCamera;
			return EventDispatching::CatchEvent;
		}
		return EventDispatching::PassEvent;
	}

	EventDispatching VreInputListener::MouseDblClick(QEvent* event)
	{
		return EventDispatching::PassEvent;
	}

	EventDispatching VreInputListener::MouseReleased(QEvent* event)
	{
		return EventDispatching::PassEvent;
	}

	EventDispatching VreInputListener::MouseMoved(QEvent* event)
	{
		if (m_mouseControlsCamera)
		{
			float centerX = m_window->width() / 2;
			float centerY = m_window->height() / 2;

			QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
			auto pos = mouseEvent->pos();
			float xpos = pos.x();
			float ypos = pos.y();

			float xoffset = xpos - centerX;
			float yoffset = centerY - ypos;

			auto camera = SceneManager::GetInstance()->GetActiveScene()->GetCamera();
			camera->MouseRotate(xoffset, yoffset);

			// reset mouse position to center
			QPoint glob = m_window->mapToGlobal(QPoint(m_window->width() / 2, m_window->height() / 2));
			auto focus = m_window->focusObject();
			if (focus == m_window)
				QCursor::setPos(glob);

			return EventDispatching::CatchEvent;
		}

		return EventDispatching::PassEvent;
	}
}