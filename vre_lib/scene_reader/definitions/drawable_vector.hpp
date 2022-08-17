#pragma once
#include "../../common.hpp"
#include "../structs/drawable_data.hpp"

namespace vre
{
	/// @brief A vector of scene reader drawables.
	using DrawableVector = std::vector<std::shared_ptr<DrawableData>>;
}