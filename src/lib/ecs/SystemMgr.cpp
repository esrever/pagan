#include "SystemMgr.h"
#include "ecs.h"
#include "ËntityMgr.h"

#include "ComponentQuery.h"

namespace pgn
{
	void cSystemMgr::from_json(const rapidjson::Value& zRoot)
	{
		const auto&  qobj = zRoot["Queries"];
		for (auto itr = qobj.MemberBegin(); itr != qobj.MemberEnd(); ++itr) 
		{
			// get a query object
			const auto& qcur = *itr;
			const auto& qname = qcur.name;
			const auto& types = qcur.value;
			component_mask_type mask;
			for (auto itr2 = types.Begin(); itr2 != types.End(); ++itr2) 
			{
				const auto& s = itr2->GetString();
				auto idx = EMGR.GetComponentTypeIndex(s);
				if(idx != 0xFFFFFFFF)
					mask.set(idx);
				else
					; // TODO: handle errors
			}
			mComponentQueries.insert( cComponentQuery(mask));
			// TODO: add names to queries! I already grabbed it
		}
		const auto&  sobj = zRoot["Systems"];
	}
}