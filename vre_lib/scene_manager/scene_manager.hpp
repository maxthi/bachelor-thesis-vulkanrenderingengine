#pragma once
#include "scene_objects/node.hpp"
#include "scene.hpp"
#include "scene_objects/camera.hpp"

namespace vre
{
	/// @brief The scene manager holds shared pointers to all scenes created by the user. There is always one scene marked as
	/// active scene that will be rendered.
	class SceneManager
	{
	public:
		/// @brief The scene manager can be obtained by the static GetInstance method. However the class itself only stores a weak pointer to the reference,
		/// and the vulkan core stores a strong one. As soon as the vulkan core is deleted, it will delete the scene manager.
		static std::shared_ptr<SceneManager> GetInstance();

		SceneManager() = default;

		/// @brief Add a scene to the scene manager.
		/// @param scene - The scene to add.
		void AddScene(std::shared_ptr<Scene> scene);

		/// @brief Set the active scene. The active scene is the one which is rendered.
		/// @param scene - The shared pointer to the scene that shoukd be active.
		void SetActiveScene(std::shared_ptr<Scene> scene);

		/// @brief Returns the shared ptr to the scene thats currently set as active scene.
		std::shared_ptr<Scene> GetActiveScene() { return m_activeScene.lock(); }

		/// @brief Creates a scene reader object for a scene.
		/// @param scene - The to create the scene reader for.
		void CreateSceneReader(std::shared_ptr<Scene> scene);

	private:
		/// @brief The weak ptr to the instance.
		static inline std::weak_ptr<SceneManager> s_sceneManager{};

		/// @brief The vector that holds all scenes added to the scene manager.
		std::vector<std::shared_ptr<Scene>> m_scenes;

		/// @brief The currently active scene.
		std::weak_ptr<Scene> m_activeScene;
	};
}