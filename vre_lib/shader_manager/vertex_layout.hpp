#pragma once
#include "../common.hpp"
#include "../scene_manager/drawable_properties/mesh.hpp"
#include "../scene_reader/structs/vertex_and_index_buffer.hpp"

namespace vre
{
	class Drawable;

	/// @brief Describes how input vertex data is structed.
	/// Can be subclassed and overwritten for custom vertex structures.
	/// A method to fill the vertex by mesh data needs to be provided by the subclassing class.
	class VertexLayout
	{
	public:
		virtual ~VertexLayout() = default;
		std::vector<VkVertexInputBindingDescription>& GetVertexInputBindingDescriptions() { return m_bindingDescriptions; }
		std::vector<VkVertexInputAttributeDescription>& GetVertexInputAttributeDescriptions() { return m_attributeDescriptions; }
		
		/// @brief The most important function of the vertex layout. Responsible for writing the mesh data in the desired format
		/// into the buffer.
		/// @param drawable - The drawable holding the mesh information.
		virtual std::shared_ptr<VertexAndIndexBuffer> CreateVertexAndIndexBuffer(std::shared_ptr<Drawable> drawable) = 0;

	protected:
		std::vector<VkVertexInputBindingDescription> m_bindingDescriptions;
		std::vector<VkVertexInputAttributeDescription> m_attributeDescriptions;
	};
}