#include "scene_manager.hpp"
#include "../scene_reader/scene_reader.hpp"

namespace vre
{
	std::shared_ptr<SceneManager> SceneManager::GetInstance()
	{
		std::shared_ptr<SceneManager> sm = s_sceneManager.lock();
		if (sm == nullptr)
		{
			sm = std::make_shared<SceneManager>();
			s_sceneManager = sm;
		}
		return sm;
	}

	void SceneManager::AddScene(std::shared_ptr<Scene> scene)
	{
		m_scenes.push_back(scene);
		CreateSceneReader(scene);
	}

	void SceneManager::SetActiveScene(std::shared_ptr<Scene> scene)
	{
		m_activeScene = scene;
	}

	void SceneManager::CreateSceneReader(std::shared_ptr<Scene> scene)
	{
		if (scene->GetSceneReader() == nullptr)
		{
			scene->SetSceneReader(std::make_shared<SceneReader>(scene));
		}
		else
		{
			log("SceneManager::CreateSceneReader: Attempted to create scene reader for scene but scene reader was already created.");
		}
	}
}