#include "drawable_property.hpp"

namespace vre
{
	DrawablePropertyId DrawableProperty::GetUniquePropertyId()
	{
		return s_propertyID++;
	}

}