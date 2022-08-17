#include "camera.hpp"
#include <QtMath>
#include "../../vulkan_manager/vulkan_manager.hpp"
#include "../../core/vulkan_window.hpp"

namespace vre
{
	Camera::Camera(uint32_t width, uint32_t height) :
		m_width(width),
		m_height(height)
	{
		if (width == 0 && height == 0)
		{
			// no dimensions for the camera are set
			// so the swapchain dimensions are used
			auto swapchainRecreationCallback = [this](bool swapchainSizeChanged)
			{
				const VkExtent2D* extent = &VulkanManager::GetInstance()->GetSwapchainInfo()->extent2D;
				m_width = extent->width;
				m_height = extent->height;
				UpdateProjectionMatrix();
			};

			swapchainRecreationCallback(false);

			VulkanManager::GetInstance()->AddSwapchainRecreationCallback(swapchainRecreationCallback);
		}
	}

	void Camera::MouseRotate(float xOffset, float yOffset)
	{
		static float yaw = qRadiansToDegrees(atan2(m_cameraDirection.z(), m_cameraDirection.x()));
		static float pitch = qRadiansToDegrees(asin(m_cameraDirection.y()));

		float sensitivity = 0.1f;
		xOffset *= sensitivity;
		yOffset *= sensitivity;

		yaw += xOffset;
		pitch += yOffset;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
			

		QVector3D direction;
		direction.setX(cos(qDegreesToRadians(yaw)) * cos(qDegreesToRadians(pitch)));
		direction.setY(sin(qDegreesToRadians(pitch)));
		direction.setZ(sin(qDegreesToRadians(yaw)) * cos(qDegreesToRadians(pitch)));
		direction.normalize();
		m_cameraDirection = direction;
		UpdateViewMatrix();
	}

	void Camera::Update()
	{
		if (m_moveForwards)
		{
			MoveForwards(0.05f);
		}
		else if (m_moveBackwards)
		{
			MoveBackwards(0.05f);
		}

		if (m_moveLeft)
		{
			MoveLeft(0.03f);
		}
		else if (m_moveRight)
		{
			MoveRight(0.03f);
		}
	}

	void Camera::MoveForwards(float distance)
	{
		m_cameraEye += distance * m_cameraDirection;
		cameraMovementUpdate();
	}

	void Camera::MoveBackwards(float distance)
	{
		m_cameraEye -= distance * m_cameraDirection;
		cameraMovementUpdate();
	}

	void Camera::MoveLeft(float distance)
	{
		m_cameraEye -= QVector3D::crossProduct(m_cameraDirection, m_cameraUp).normalized() * distance;
		cameraMovementUpdate();
	}

	void Camera::MoveRight(float distance)
	{
		QVector3D crossProduct = QVector3D::crossProduct(m_cameraDirection, m_cameraUp);
		crossProduct.normalize();
		m_cameraEye += crossProduct * distance;
		cameraMovementUpdate();
	}

	QMatrix4x4* Camera::GetViewMatrix()
	{
		return &m_view;
	}

	QMatrix4x4* Camera::GetProjectionMatrix()
	{
		return &m_proj;
	}

	void Camera::SetPosition(float x, float y, float z)
	{
		m_cameraEye.setX(x);
		m_cameraEye.setY(y);
		m_cameraEye.setZ(z);
		UpdateViewMatrix();
	}

	void Camera::SetCameraDirection(float x, float y, float z)
	{
		m_cameraDirection.setX(x);
		m_cameraDirection.setY(y);
		m_cameraDirection.setZ(z);
		UpdateViewMatrix();
	}

	QVector3D* Camera::GetCameraEye()
	{
		return &m_cameraEye;
	}

	QVector3D* Camera::GetCameraCenter()
	{
		return &m_cameraCenter;
	}

	QVector3D* Camera::GetCameraUp()
	{
		return &m_cameraUp;
	}

	QVector3D* Camera::GetCameraFront()
	{
		return &m_cameraDirection;
	}

	void Camera::UpdateViewMatrix()
	{
		m_view.setToIdentity();
		m_view.lookAt(m_cameraEye, m_cameraEye + m_cameraDirection, m_cameraUp);
	}

	void Camera::UpdateProjectionMatrix()
	{
		m_proj.setToIdentity();
		float aspectRatio = (float)m_width / (float)m_height;
		m_proj.perspective(45.0f, aspectRatio, 0.1f, 1000.0f);
	}

	void Camera::cameraMovementUpdate()
	{
		UpdateViewMatrix();
		m_scene->CallbackCameraMoved();
	}

}