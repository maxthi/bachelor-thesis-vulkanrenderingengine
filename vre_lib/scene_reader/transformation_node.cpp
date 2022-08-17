#include "transformation_node.hpp"
#include "scene_reader.hpp"

namespace vre
{
	TransformationNode::TransformationNode(std::shared_ptr<Node> node, SceneReader* sceneReader) :
		m_sceneReader(sceneReader)
	{
		m_node = node;
		m_localTransform = node->GetTransformation();
		m_worldTransform = std::make_shared<QMatrix4x4>();
		m_finalNodeTransform = std::make_shared<QMatrix4x4>();
		m_finalDrawableTransform = std::make_shared<QMatrix4x4>();
		setModelTransformation(node->GetDrawable());
		UpdateFinalTransform();
	}

	std::shared_ptr<QMatrix4x4> TransformationNode::GetFinalDrawableTransformation()
	{
		return m_finalDrawableTransform;
	}

	void TransformationNode::AddChild(std::shared_ptr<TransformationNode> node)
	{
		m_children.push_back(node);
		node->setWorldTransform(m_finalNodeTransform);
	}

	void TransformationNode::UpdateFinalTransform()
	{
		// calculate final transformation of the node
		*m_finalNodeTransform = *m_worldTransform * (*m_localTransform);

		if (m_modelTransformation != nullptr)
		{
			*m_finalDrawableTransform = (*m_finalNodeTransform) * (*m_modelTransformation->Get());
		}
		else
		{
			*m_finalDrawableTransform = (*m_finalNodeTransform);
		}

		// populate final transform as world transform to children
		for (const auto& child : m_children)
		{
			child->setWorldTransform(m_finalNodeTransform);
		}

	}

	void TransformationNode::setWorldTransform(std::shared_ptr<QMatrix4x4> worldTransform)
	{
		m_worldTransform = worldTransform;
		UpdateFinalTransform();
	}

	void TransformationNode::setModelTransformation(std::shared_ptr<Drawable> drawable)
	{
		if (drawable == nullptr)
		{
			return;
		}

		m_modelTransformation = drawable->GetProperty<ModelTransformation>();
	}
	
}