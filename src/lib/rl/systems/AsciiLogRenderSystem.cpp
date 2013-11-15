#include "AsciiLogRenderSystem.h"

#include <curses.h>

#include <ecs/Component.h>
#include <ecs/EntityComponents.h>

#include <rl/components/AsciiWindow.h>
#include <rl/components/Logger.h>
#include <rl/components/LoggerListener.h>

namespace pgn
{
	void cAsciiLogRenderSystem::Process()
	{
		for (auto e : mQueryWin->Entities())
		{
			// First, get the window 
			std::shared_ptr< cComponent<cAsciiWindow>> asciiwin_ptr;
			std::shared_ptr< cComponent<cLoggerListener>> loglisten_ptr;
			auto ec = ECS.mEntityMgr->GetComponents().find(e);
			assert(ec != ECS.mEntityMgr->GetComponents().end());
			ec->second.GetComponent(asciiwin_ptr);
			auto win = *asciiwin_ptr->mData.mWindow.get();

			// ... and the log list that it listens to
			ECS.mEntityMgr->GetComponents().find(e)->second.GetComponent(loglisten_ptr);
			

			// Now, look at all the loggers that we have
			std::shared_ptr< cComponent<cLogger>> log_ptr;
			for (auto elog : mQueryLog->Entities())
			{
				auto eclog = ECS.mEntityMgr->GetComponents().find(e);
				assert(eclog != ECS.mEntityMgr->GetComponents().end());
				eclog->second.GetComponent(log_ptr);

				// for each of the log list names
				for (const auto& logname : loglisten_ptr->mData.mLogTags)
				{
					// if logger matches the log list name
					auto logname_ents = ECS.mEntityMgr->TaggedEntities().find(logname);
					if (logname_ents != ECS.mEntityMgr->TaggedEntities().end())
					{
						// Write log line!
						wprintw(win, "Listening to %s\n",logname.c_str());
					}
				}
			}
			wrefresh(win);
		}
	}

	bool cAsciiLogRenderSystem::from_json(const rapidjson::Value& zRoot)
	{
		cSystemBase::from_json(zRoot);

		auto b0 = LoadQuery(mQueryWin, zRoot, "QueryWin");
		auto b1 = LoadQuery(mQueryLog, zRoot, "QueryLog");
		return b0 && b1;
	}

	void cAsciiLogRenderSystem::to_json(JsonWriter& zRoot) const
	{
		zRoot.StartObject();
		zRoot.String("Base");
		cSystemBase::to_json(zRoot);
		JsonWriter_AddMember("QueryWin", mQueryWin, zRoot);
		JsonWriter_AddMember("QueryLog", mQueryLog, zRoot);
		zRoot.EndObject();
	}
}