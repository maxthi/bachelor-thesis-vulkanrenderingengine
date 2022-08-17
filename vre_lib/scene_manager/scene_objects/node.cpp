#include "node.hpp"
#include "../scene.hpp"

namespace vre
{
	Node::Node()
	{
		m_localTransform = std::make_shared<QMatrix4x4>();
	}

	void Node::UpdateTransformation()
	{
		for (auto& transformationNode : m_transformationNodes)
		{
			// the pointer to the local transformation is shared with the transformation node.
			// calling update will update the finale transformations based on the local one.
			transformationNode->UpdateFinalTransform();
		}
	}

	void Node::AddChild(std::shared_ptr<Node> node)
	{
		m_children.push_back(node);

		for(auto& scenePair : m_scenes)
		{
			scenePair.first->CallNodeAdded(node, this);
			node->AddScene(scenePair.first);
		}
	}

	void Node::RemoveChild(std::shared_ptr<Node> node)
	{
		// wait for command buffers to finish execution before deleting the buffers
		VulkanInterface::vkDeviceWaitIdle(VulkanManager::GetInstance()->GetDevice());

		checkPtr(node, "Node::RemoveChild: Parameter node is a nullptr!");
		for (auto it = m_children.begin(); it != m_children.end(); it++)
		{
			auto& childNode = *it;
			if (node == childNode)
			{
				m_children.erase(it);
				for (auto scenePair : m_scenes)
				{
					scenePair.first->CallNodeRemoved(node, this);
					node->RemoveScene(scenePair.first);
				}
				return;
			}
		}
		err(std::string("Node::RemoveChild: The child you attempted to remove was not found! Nodename: ") + std::string(node->Name));
	}

	void Node::AddScene(Scene* scene)
	{
		auto it = m_scenes.find(scene);
		if (it == m_scenes.end())
		{
			m_scenes.insert({ scene, 1 });
		}
		else
		{
			it->second++;
		}

		// propagate scene to children
		for (auto& child : m_children)
		{
			child->AddScene(scene);
		}
	}

	void Node::RemoveScene(Scene* scene)
	{
		auto it = m_scenes.find(scene);
		if (it != m_scenes.end())
		{
			it->second--;
			if (it->second == 0)
			{
				m_scenes.erase(it);
			}
		}
		else
		{
			err("Node::RemoveScene: The nodes scene map does not contain the scene you attempt to remove!");
		}

		// remove scene from children
		for (auto& child : m_children)
		{
			child->RemoveScene(scene);
		}
	}

	void Node::GetAllChildren(std::unordered_set<std::shared_ptr<Node>>& childrenOutput, std::shared_ptr<Node> child)
	{
		std::list<std::shared_ptr<Node>>* children;
		if(child != nullptr)
		{
			children = &child->m_children;
		}
		else
		{
			children = &m_children;
		}
		for (auto& child : *children)
		{
			GetAllChildren(childrenOutput, child);
			childrenOutput.insert(child);
		}
	}

}
