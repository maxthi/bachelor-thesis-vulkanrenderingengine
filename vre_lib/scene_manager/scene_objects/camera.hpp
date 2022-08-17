#pragma once
#include "../../common.hpp"

namespace vre
{
	class Scene;

	class Camera
	{
	public:

		/// @brief Creates a camera. If the width and height are left to zero,
		/// the camera will use the resolution of the swapchain images to calculate the projection matrix.
		/// @param width - The width to calculate the projection matrix.
		/// @param height - The height to calculate the projection matrix.
		Camera(uint32_t width = 0, uint32_t height = 0);

		/// @brief Tell the camera which scene it belongs to.
		/// If the camera moves, object rendering in the scene (mostly for transparent ones) needs to be reevaluated.
		/// @param scene - The scene that the camera belongs to.
		void SetScene(Scene* scene) { m_scene = scene; };

		void SetMoveForwards(bool moving) { m_moveForwards = moving; }
		void SetMoveBackwards(bool moving) { m_moveBackwards = moving; }
		void SetMoveLeft(bool moving) { m_moveLeft = moving; }
		void SetMoveRight(bool moving) { m_moveRight = moving; }
		void MouseRotate(float xOffset, float yOffset);

		void Update();

		void MoveForwards(float distance);
		void MoveBackwards(float distance);
		void MoveLeft(float distance);
		void MoveRight(float distance);

		QMatrix4x4* GetViewMatrix();
		QMatrix4x4* GetProjectionMatrix();

		void SetPosition(float x, float y, float z);
		void SetCameraDirection(float x, float y, float z);

		QVector3D* GetCameraEye();
		QVector3D* GetCameraCenter();
		QVector3D* GetCameraUp();
		QVector3D* GetCameraFront();

		void UpdateViewMatrix();
		void UpdateProjectionMatrix();

	protected:
		Scene* m_scene{};

		/// @brief The camera dimension in x.
		uint32_t m_width{};

		/// @brief The camera dimension in y.
		uint32_t m_height{};

		QVector3D m_cameraEye{ 0.0f, 0.0f, 3.0f };
		QVector3D m_cameraDirection{ 0.0f, 0.0f, -1.0f };
		QVector3D m_cameraUp{ 0.0f, 1.0f, 0.0f };
		QVector3D m_cameraCenter{ 0.0f, 0.0f, 0.0f };

		QMatrix4x4 m_view;
		QMatrix4x4 m_proj;

		bool m_moveForwards{ false };
		bool m_moveBackwards{ false };
		bool m_moveRight{ false };
		bool m_moveLeft{ false };

		void cameraMovementUpdate();
	};
}