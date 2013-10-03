#include "SystemMgr.h"

#include "ComponentQuery.h"

namespace pgn
{
	void cSystemMgr::from_json(const rapidjson::Value& zRoot)
	{
		const auto&  qobj = zRoot["Queries"];
		for (auto itr = qobj.Begin(); itr != qobj.End(); ++itr) 
		{
			const auto& qcur = *itr;
			//qcur.GetName
		}
		const auto&  sobj = zRoot["Systems"];
	}
}