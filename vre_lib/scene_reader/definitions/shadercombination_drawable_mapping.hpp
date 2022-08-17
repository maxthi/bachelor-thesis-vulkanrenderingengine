#pragma once
#include "../../common.hpp"
#include "drawable_vector.hpp"

namespace vre
{
	using ShaderCombinationDrawableMapping = std::unordered_map<uint64_t, std::shared_ptr<DrawableVector>>;
}