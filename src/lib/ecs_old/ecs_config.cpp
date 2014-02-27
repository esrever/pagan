#pragma once

#include "ecs_config.h"
#include "ecs.h"

namespace pgn
{

	//------------------------------------------------------------------------
	template<>
	void to_json<component_mask_type>(const component_mask_type& zObj, JsonWriter& zRoot)
	{
		zRoot.StartArray();
		for (size_t i = 0; i < zObj.size(); ++i)
		{
			if (zObj.at(i))
				zRoot.String(ECS.mEntityMgr->GetComponentTypeIndexAll().at(i).name());
		}
		zRoot.EndArray();
	}

}