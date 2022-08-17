#include "scene.hpp"
#include "../vulkan_manager/vulkan_manager.hpp"
#include "../scene_manager/scene_manager.hpp"
#include "../scene_reader/scene_reader.hpp"

namespace vre
{
	Scene::Scene(std::shared_ptr<Node> rootNode, std::shared_ptr<Camera> camera) :
		m_rootNode(rootNode)
	{
		if (camera == nullptr)
		{
			camera = std::make_shared<Camera>();
		}
		m_camera = camera;
		m_camera->SetScene(this);
		rootNode->AddScene(this);
	}

	std::shared_ptr<Node> Scene::GetRootNode()
	{
		return m_rootNode;
	}

	void Scene::AddNodeAddedCallback(std::function<void(std::shared_ptr<Node>addedNode, Node* parentNode)> callback)
	{
		m_nodeAddedCallbacks.push_back(callback);
	}

	void Scene::AddNodeRemovedCallback(std::function<void(std::shared_ptr<Node>removedNode, Node* parentNode)> callback)
	{
		m_nodeRemovedCallbacks.push_back(callback);
	}

	void Scene::CallNodeAdded(std::shared_ptr<Node> addedNode, Node* parentNode)
	{
		for (auto& callback : m_nodeAddedCallbacks)
		{
			callback(addedNode, parentNode);
		}
	}

	void Scene::CallNodeRemoved(std::shared_ptr<Node> removedNode, Node* parentNode)
	{
		for (auto& callback : m_nodeRemovedCallbacks)
		{
			callback(removedNode, parentNode);
		}
	}

	void Scene::CallbackCameraMoved()
	{
		m_sceneReader->UpdateTransparentDrawableOrder();
	}
}