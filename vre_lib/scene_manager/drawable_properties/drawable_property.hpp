#pragma once
#include "../../common.hpp"

namespace vre
{

	using DrawablePropertyId = uint32_t;

	struct DrawableData;

	/// @brief All drawable properties inherit from this class.
	/// Each subclass needs to implement a static method, called GetClassPropertyID(), where
	/// DrawableProperty::GetPropertyClassId() is called to obtain a unique Identifier.
	/// This ID is stored in the protected m_propertyId variable, so GetObjectPropertyId returns
	/// a valid result.
	class DrawableProperty
	{
	public:
		/// @brief Used by DrawableProperty subclasses to obtain an arbitrary unique identifier.
		/// @return The Identifier of the property, aka propertyID.
		static DrawablePropertyId GetUniquePropertyId();

		/// @brief Default constructor.
		DrawableProperty() = default;

		/// @brief Default destructor.
		virtual ~DrawableProperty() = default;

		/// @brief Returns the property Id of this drawable property class.
		/// This method is only virtual because one virtual method is required for polymorphing(?)
		/// @return 
		virtual DrawablePropertyId GetObjectPropertyId() { return m_propertyId; }

		/// @brief Called each time a drawable uses this property to allocate the properties memory.
		/// Either allocate one memory block for all drawables or allocate individual memory per id.
		/// @param drawableDataId - The drawable data id is unique, so a property can either
		/// allocate memory once for each instance, a reused texture for example. Or it can store
		/// memory for each drawable data id seperately.
		virtual void AllocateMemory(std::shared_ptr<DrawableData>& drawableData) {};

		/// @brief Called by each scene reader using this property, to free the resources allocated beforehand.
		/// @param drawableDataId - The unique id to identify the drawable that used the memory, see AllocateMemory.
		virtual void FreeMemory(std::shared_ptr<DrawableData>& drawableData) {};


	protected:
		DrawablePropertyId m_propertyId{ 0 };

	private:
		static inline DrawablePropertyId s_propertyID{ 0 };
	};
}