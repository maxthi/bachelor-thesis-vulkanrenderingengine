#pragma once
#include "../../common.hpp"
#include "../drawable_properties/mesh.hpp"
#include "../drawable_properties/texture_2d.hpp"
#include "../../shader_manager/shaders/vertex_shader.hpp"
#include "../../shader_manager/shaders/fragment_shader.hpp"

namespace vre
{
    class Node;
	/// @brief Contains all information for drawing
	class Drawable
	{
	public:
		Drawable() { };

		/// @brief If transparency is set to true, the drawable is sorted by distance before rendering it.
		void SetTransparency(bool isTransparent) { m_isTransparent = isTransparent; }

		/// @brief Returns whether the drawable is to be handled as transparent.
		bool IsTransparent() { return m_isTransparent; }

		/// @brief Get the property of class T at position of the index variable.
		/// @tparam T - The property to return.
		/// @param pos - The position in the vector of properties of type T added to this drawable.
		/// @return The property of type T at position pos if existent, else nullptr.
		template <typename T>
		std::shared_ptr<T> GetProperty(size_t pos = 0)
		{
			DrawablePropertyId propertyId = T::GetClassPropertyId();
			auto it = m_properties.find(propertyId);
			if (it == m_properties.end())
			{
				return nullptr;
			}
			
			if (pos >= it->second.size())
			{
				return nullptr;
			}
			return std::dynamic_pointer_cast<T>(it->second.at(pos));
		}

		/// @brief Gets a pointer to the vector containing all properties of on template type.
		/// @tparam T - The property to get.
		/// @return The pointer to the vector containing the properties.
		template <typename T>
		std::vector<std::shared_ptr<T>>* GetPropertyVector()
		{
			DrawablePropertyId propertyId = T::GetClassPropertyId();
			auto it = m_properties.find(propertyId);
			if (it == m_properties.end())
			{
				return nullptr;
			}
			
			// return line means take ptr of vector of shared_ptr<DrawableProperty>'s and cast to
			// ptr of vector of shared_ptr<T>'s
			return (std::vector<std::shared_ptr<T>>*) &(it->second);
		}

		/// @brief Returns the number of properties added by type property id.
		/// @tparam T - The property class to count.
		/// @return Size of the vector containing property objects.
		template <typename T>
		size_t GetPropertyCount()
		{
			DrawablePropertyId propertyId = T::GetClassPropertyId();
			auto it = m_properties.find(propertyId);
			if (it == m_properties.end())
			{
				return 0;
			}
			return it->second.size();
		}

		/// @brief Adds the property to the drawable.
		/// @tparam T - The class of the property to be added. Needs to be a subclass of DrawableProperty.
		/// @param property - The shader ptr to the property object.
		/// @return The position of the new object in the vector.
		template <typename T>
		size_t AddProperty(std::shared_ptr<T> property)
		{
			DrawablePropertyId propertyId = T::GetClassPropertyId();
			auto it = m_properties.find(propertyId);
			if (it == m_properties.end())
			{
				std::vector<std::shared_ptr<DrawableProperty>> propertyVector;
				propertyVector.push_back(property);
				m_properties.insert({ propertyId, propertyVector });
				// property at index 0
				return 0;
			}

			it->second.push_back(property);
			// property at index size-1
			return it->second.size() - 1;
		}

		const std::unordered_map<DrawablePropertyId, std::vector<std::shared_ptr<DrawableProperty>>>* GetAllProperties() { return &m_properties; }

	private:
		/// @brief A drawable can have multiple properties attached. This map sorts the vector of drawable properties by the drawable property id.
		std::unordered_map<DrawablePropertyId, std::vector<std::shared_ptr<DrawableProperty>>> m_properties;

		/// @brief Whether the drawable should be considered a transparent object.
		/// If true its rendering ordered is adjusted to camera distance.
		bool m_isTransparent{ false };
	};
}