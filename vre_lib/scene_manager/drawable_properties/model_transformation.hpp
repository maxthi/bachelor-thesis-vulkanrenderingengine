#pragma once
#include "../../common.hpp"
#include "drawable_property.hpp"

namespace vre
{
	/// @brief The model transformation affects the drawable and is visible to all drawables its attached to.
	class ModelTransformation : public DrawableProperty
	{
	public:
		static DrawablePropertyId GetClassPropertyId() {
			static DrawablePropertyId propertyId = DrawableProperty::GetUniquePropertyId();
			return propertyId;
		}

		ModelTransformation(std::shared_ptr<QMatrix4x4> transformation)
		{ 
			m_propertyId = ModelTransformation::GetClassPropertyId();
			m_transformation = transformation;
		}

		/// @brief Set the transformation matrix.
		/// @param transformation - A pointer to the matrix.
		void Set(std::shared_ptr<QMatrix4x4> transformation) { m_transformation = transformation; }

		/// @brief Get the transformation matrix.
		/// @return A pointer to the matrix.
		std::shared_ptr<QMatrix4x4> Get() { return m_transformation; }

	private:
		/// @brief The matrix holding the model transformation.
		std::shared_ptr<QMatrix4x4> m_transformation;
	};
}