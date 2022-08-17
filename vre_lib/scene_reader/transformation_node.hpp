#pragma once
#include "../common.hpp"
#include "../scene_manager/scene_objects/drawable.hpp"
#include "../scene_manager/drawable_properties/model_transformation.hpp"
#include "../scene_manager/scene_objects/node.hpp"


namespace vre
{

	class SceneReader;

	/// @brief The transformation nodes help to create a transformation tree for each scene reader.
	/// A transformation tree is used to calculate the final transformation for each drawable.
	/// A single scene can have multiple transformation trees.
	/// This is due the possibility of render targets to reuse a node in the scene as their root node,
	/// hence setting the scene readers origin to this node.
	class TransformationNode
	{
	public:
		/// @brief Create a transformation node
		/// @param localTransformation - A pointer to the local transformation of the real node.
		/// @return 
		TransformationNode(std::shared_ptr<Node> node, SceneReader* sceneReader);

		/// @brief The node that the transformation node represents.
		std::weak_ptr<Node> GetNode() { return m_node; }

		/// @brief The local and the world transform combined to a final transformation for the node, not considering
		/// any model transformations attached to the attached drawable.
		std::shared_ptr<QMatrix4x4> GetFinalNodeTransformation() { return m_finalNodeTransform; }

		/// @brief Pointer to a vector containing all children attached to this node.
		std::vector<std::shared_ptr<TransformationNode>>* GetChildren() { return &m_children; }

		/// @brief Returns the finale transformation for this drawable, containing the final transformation of the node
		/// as well as the transformation of the attached model.
		std::shared_ptr<QMatrix4x4> GetFinalDrawableTransformation();

		/// @brief Add a child node to this node.
		void AddChild(std::shared_ptr<TransformationNode> node);

		/// @brief Compute the resulting final transformation for the node and the drawable.
		/// Update on either world or local transform updates.
		/// Populated to child nodes as world transform.
		void UpdateFinalTransform();

		/// @brief Returns the scene reader that owns the tree which the transformation node belongs to.
		/// @return The scene reader pointer.
		SceneReader* GetSceneReader() { return m_sceneReader; }

	private:
		/// @brief The node in the scene tree this transformation node is attached too.
		std::weak_ptr<Node> m_node;

		/// @brief Transformation of the model if its set to the drawable.
		std::shared_ptr<ModelTransformation> m_modelTransformation;

		/// @brief A pointer to the scene reader that the transformation node belongs to.
		SceneReader* m_sceneReader{};

		std::vector<std::shared_ptr<TransformationNode>> m_children{};

		/// @brief Changes only to this node
		std::shared_ptr<QMatrix4x4> m_localTransform{};

		/// @brief Affection of this node through previous transformations.
		std::shared_ptr<QMatrix4x4> m_worldTransform{};

		/// @brief The local and the world transform combined to a final transformation for the node.
		/// This transform is passed to all child transforms.
		std::shared_ptr<QMatrix4x4> m_finalNodeTransform{};

		/// @brief The final transform combined with the transform of the modell, resulting in the final transform
		/// for the drawable.
		std::shared_ptr<QMatrix4x4> m_finalDrawableTransform{};

		/// @brief Set the transformation that has been created by all parent nodes.
		/// @param worldTransform - Pointer to the transformation created by the parents.
		void setWorldTransform(std::shared_ptr<QMatrix4x4> worldTransform);

		/// @brief Get the model transformation property of the drawable if not nullptr and store it as member variable.
		/// @param drawable - The drawable holding the model transformation property.
		void setModelTransformation(std::shared_ptr<Drawable> drawable);
	};
}
