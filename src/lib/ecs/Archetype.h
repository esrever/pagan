#pragma once

#include <vector>
#include <string>
#include "ecs_config.h"

namespace pgn
{
	struct cArchetype
	{
		std::string				 mName;
		component_mask_type		 mMask;
		std::vector<std::string> mTags;
	};
}