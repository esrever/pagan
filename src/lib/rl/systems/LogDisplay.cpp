#include "LogDisplay.h"

#include <ecs/Event.h>
#include <ecs/Component.h>
#include <ecs/EntityData.h>

#include <rl/components/Log.h>

namespace pgn
{
	namespace sys
	{
		cLogDisplay::cLogDisplay()
		{

		}

		void cLogDisplay::Process()
		{
			// For each log
			std::vector<std::shared_ptr< cComponent<cmp::cLog>>> log_ptrs(mQueryLog->Entities().size());
			int i = 0;
			for (auto e : mQueryLog->Entities())
			{
				auto ec = ECS.mEntityMgr->GetEntityData().find(e);
				assert(ec != ECS.mEntityMgr->GetEntityData().end());
				ec->second.mComponents.GetComponent(log_ptrs.at(i));
				++i;
			}

			// For each log output
			// TODO:
		}

		bool cLogDisplay::from_json(const rapidjson::Value& zRoot)
		{
			cSystemBase::from_json(zRoot);

			auto b0 = LoadQuery(mQueryLog, zRoot, "QueryLog");
			return b0;
		}

		void cLogDisplay::to_json(JsonWriter& zRoot) const
		{
			zRoot.StartObject();
			zRoot.String("Base");
			cSystemBase::to_json(zRoot);
			JsonWriter_AddMember("QueryLog", mQueryLog, zRoot);
			zRoot.EndObject();
		}
	}
}