#pragma once
#include "../../common.hpp"
#include "drawable.hpp"
#include "../../scene_reader/transformation_node.hpp"

namespace vre
{
	class TransformationNode;
	class Scene;

	/// @brief Nodes build the scene graph. Do not attach a node twice.
	class Node
	{
	public:
		Node();

		/// @brief Returns the local transformation of the matrix. See the description of the local transformation for more information.
		/// @return The shared pointer to the QMatrix4x4 that holds the local node transformation.
		std::shared_ptr<QMatrix4x4> GetTransformation() { return m_localTransform; }

		/// @brief This needs to be called after any change to the transformation has been done in order to update
		/// all dependent transformation trees.
		void UpdateTransformation();

		/// @brief Add a node as child.
		/// @param node - The node to add.
		void AddChild(std::shared_ptr<Node> node);

		/// @brief Removes a child node.
		/// @param node - The node to remove.
		/// @param updateScene - Whether removing the node should trigger the scene changed callbacks. Necessary if the scene
		/// has been loaded by the scene reader.
		void RemoveChild(std::shared_ptr<Node> node);

		/// @brief Returns the drawable attached to the node. A drawable is optional.
		/// @return - The drawable if attached or a nullptr.
		std::shared_ptr<Drawable> GetDrawable() { return m_drawable; }

		/// @brief Set the drawable for the node.
		/// @param drawable - The drawable to set.
		void SetDrawable(std::shared_ptr<Drawable> drawable) { m_drawable = drawable; }

		/// @brief Returns a pointer to the list of all child nodes attached to the node.
		/// @return - Pointer to the list of children.
		std::list<std::shared_ptr<Node>>* GetChildren() { return &m_children; }

		/// @brief Add a transformation node to this node. If the local transformation is changed, the transformation nodes must be updated.
		/// @param transformationNode 
		void AddTransformationNode(std::shared_ptr<TransformationNode> transformationNode) { m_transformationNodes.push_back(transformationNode); }

		/// @brief Provides access to the the vector that stores all transformations nodes, that belong to a node.
		/// @return Pointer the the vector storing the transformation nodes.
		std::vector<std::shared_ptr<TransformationNode>>* GetTransformationNodes() { return &m_transformationNodes; }

		/// @brief Adds a scene that contains this node. If children are added/removed to/from this node,
		/// all added scenes get informed via callback.
		/// @param scene - The scene containing the node.
		void AddScene(Scene* scene);

		/// @brief Removes the scene once from the node. Will either decrease the counter in the scenes map or if counter
		/// hits 0, will remove the scene from the map.
		/// @param scene - The scene to remove.
		void RemoveScene(Scene* scene);

		/// @brief Optional name to easily identify a drawable during debugging.
		std::string Name;

		/// @brief Iterates recursively through all children and all subchildren and inserts them into the unordered set
		/// given as parameter.
		/// @param childrenOutput - After calling the method childrenOutput contains all children and subchildren.
		/// @param child - If != nullptr, will use the children of child as start point for the iteration.
		void GetAllChildren(std::unordered_set<std::shared_ptr<Node>>& childrenOutput, std::shared_ptr<Node> child = nullptr);

	private:
		/// @brief List of all children.
		std::list<std::shared_ptr<Node>> m_children{};

		/// @brief The drawable attached to the node.
		std::shared_ptr<Drawable> m_drawable;

		/// @brief A vector holding all transformation nodes that are based on this node.
		/// Whenever changes occur to this node in terms of transformation, the transformation nodes
		/// need updates.
		std::vector<std::shared_ptr<TransformationNode>> m_transformationNodes{};

		/// @brief The local transformation is the transformation done to the node, without considering previous transfomations
		/// to the parent nodes.
		std::shared_ptr<QMatrix4x4> m_localTransform{};

		/// @brief The various secenes a node is attached to and the number of times its attached to this scene.
		/// A node can be attached to multiple scenes and mutliple times to the same scene,
		/// because it can be a childnode of a node attached to the same scene.
		std::unordered_map<Scene*, uint16_t> m_scenes{};
	};
}
