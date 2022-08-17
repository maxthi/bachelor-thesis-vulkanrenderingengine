#pragma once
#include "../common.hpp"
#include "scene_objects/node.hpp"
#include "scene_objects/camera.hpp"

namespace vre
{
	class Scene
	{
	public:
		/// @brief Constructor.
		/// @param rootNode - The root node and origin of this scene.
		/// @param camera - The camera used in the scene. If left to null a default camera is created.
		Scene(std::shared_ptr<Node> rootNode, std::shared_ptr<Camera> camera = nullptr);

		/// @brief Get the scenes root node.
		std::shared_ptr<Node> GetRootNode();

		/// @brief Returns the camera of this scene.
		std::shared_ptr<Camera> GetCamera() { return m_camera; }

		/// @brief Set the camera of this scene.
		void SetCamera(std::shared_ptr<Camera> camera) { m_camera = camera; }

		/// @brief Add callback thats called when a node is added to the scene.
		/// @param callback 
		void AddNodeAddedCallback(std::function<void(std::shared_ptr<Node> addedNode, Node* parentNode)> callback);

		/// @brief Add callback thats called when a node is removed from the scene.
		/// @param callback 
		void AddNodeRemovedCallback(std::function<void(std::shared_ptr<Node> removedNode, Node* parentNode)> callback);

		/// @brief Called by a Node to signal the scene that a node has been added.
		/// @param addedNode - The node that was added.
		/// @param parentNode - The node that obtained the new node.
		void CallNodeAdded(std::shared_ptr<Node> addedNode, Node* parentNode);

		/// @brief Called by a Node to signal the scene that a node has been removed.
		/// @param addedNode - The node that was removed.
		/// @param parentNode - The node that lost the node.
		void CallNodeRemoved(std::shared_ptr<Node> removedNode, Node* parentNode);

		/// @brief Called when the scenes camera has moved. Inform scene reader about it.
		void CallbackCameraMoved();

		/// @brief Set the scene reader that is responsible for this scene.
		/// @param sceneReader - The scene reader.
		void SetSceneReader(std::shared_ptr<SceneReader> sceneReader) { m_sceneReader = sceneReader; }

		/// @brief Returns the scene reader related to the scene. Can be a nullptr.
		std::shared_ptr<SceneReader> GetSceneReader() { return m_sceneReader; }

	private:
		/// @brief The scene reader that creates the scenes data on gpu memory.
		std::shared_ptr<SceneReader> m_sceneReader{};

		/// @brief The node that is declared as root node for this scene.
		std::shared_ptr<Node> m_rootNode;

		/// @brief The camera looking into this scene.
		std::shared_ptr<Camera> m_camera;

		/// @brief Callbacks to be called when the scenegraph got a node added.
		std::vector<std::function<void(std::shared_ptr<Node> addedNode, Node* parentNode)>> m_nodeAddedCallbacks;

		/// @brief Callbacks to be called when the scenegraph got a node removed.
		std::vector<std::function<void(std::shared_ptr<Node> removedNode, Node* parentNode)>> m_nodeRemovedCallbacks;
	};
}