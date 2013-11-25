#include "Log.h"

#include <ecs/Event.h>
#include <ecs/Component.h>
#include <ecs/EntityData.h>

#include <rl/components/Log.h>

namespace pgn
{
	namespace sys
	{
		cLog::cLog()
		:mOnLog(Simple::slot(this, &cLog::OnLog))
		{

		}

		void cLog::OnLog(const std::string& zString)
		{ 
			for (auto e : mQuery->Entities())
			{
				std::shared_ptr< cComponent<cmp::cLog>> log_ptr;
				auto ec = ECS.mEntityMgr->GetEntityData().find(e);
				assert(ec != ECS.mEntityMgr->GetEntityData().end());
				ec->second.mComponents.GetComponent(log_ptr);
				cmp::cLog& a_log = log_ptr->mData;

				if (a_log.mMaxLineNum)
				{
					if (a_log.mLines.size() == a_log.mMaxLineNum)
					{
						a_log.mLines.erase(a_log.mLines.begin());
						// TODO: apply modifiers here, depending on the type of the logger. virtual function for text transfoermation
						a_log.mLines.push_back(zString);
					}
				}
			}
		}

		void cLog::Process()
		{
			
		}

		bool cLog::from_json(const rapidjson::Value& zRoot)
		{
			cSystemBase::from_json(zRoot);

			auto b0 = LoadQuery(mQuery, zRoot, "Query");
			return b0;
		}

		void cLog::to_json(JsonWriter& zRoot) const
		{
			zRoot.StartObject();
			zRoot.String("Base");
			cSystemBase::to_json(zRoot);
			JsonWriter_AddMember("Query", mQuery, zRoot);
			zRoot.EndObject();
		}
	}
}