#include "Log.h"

#include <ecs/Event.h>
#include <ecs/Component.h>
#include <ecs/EntityData.h>

#include <rl/components/Log.h>
#include <rl/components/TextWindow.h>
#include <rl/components/OutStream.h>

namespace pgn
{
	namespace sys
	{
		cLog::cLog()
		:mOnLog(Simple::slot(this, &cLog::OnLog))
		{

		}

		void cLog::OnLog(const std::string& zLoggerName, const std::string& zString)
		{ 
			for (auto e : mQueryLog->Entities())
			{
				// Get the log
				std::shared_ptr< cComponent<cmp::cLog>> log_ptr;
				auto ec = ECS.mEntityMgr->GetEntityData().find(e);
				assert(ec != ECS.mEntityMgr->GetEntityData().end());
				
				// Move to the next logger if it's not the one we're looking for
				if (ec->second.mName != zLoggerName)
					continue;

				// Get the log
				ec->second.mComponents.GetComponent(log_ptr);
				cmp::cLog& a_log = log_ptr->mData;

				// Do we have available lines to write?
				if (a_log.mMaxLineNum)
				{
					// if we're full, erase the oldest line
					if (a_log.mLines.size() == a_log.mMaxLineNum)
						a_log.mLines.erase(a_log.mLines.begin());

					// TODO: apply modifiers here, depending on the type of the logger. virtual function for text transfoermation

					// write the log line
					a_log.mLines.push_back(zString);
				}

				// Convert all log lines to a single string for update
				auto log_string = pystring::join("\n", a_log.mLines);

				// Find out all the listeners
				for (auto e : mQueryLogListener->Entities())
				{
					auto ec = ECS.mEntityMgr->GetEntityData().find(e);
					assert(ec != ECS.mEntityMgr->GetEntityData().end());
					// Move to the next log listener if it's not the one we're looking for
					if (ec->second.mName != zLoggerName)
						continue;

					// Does it have a TextWin?
					std::shared_ptr< cComponent<cmp::cTextWindow>> twin_ptr;
					ec->second.mComponents.GetComponent(twin_ptr);
					if (twin_ptr)
					{

					}

					// Does it have an OutStream?
					std::shared_ptr< cComponent<cmp::cOutStream>> os_ptr;
					ec->second.mComponents.GetComponent(os_ptr);
					if (os_ptr)
					{

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

			auto b0 = LoadQuery(mQueryLog, zRoot, "QueryLog");
			auto b1 = LoadQuery(mQueryLogListener, zRoot, "QueryLogListener");
			return b0 && b1;
		}

		void cLog::to_json(JsonWriter& zRoot) const
		{
			zRoot.StartObject();
			zRoot.String("Base");
			cSystemBase::to_json(zRoot);
			JsonWriter_AddMember("QueryLog", mQueryLog, zRoot);
			JsonWriter_AddMember("QueryLogListener", mQueryLogListener, zRoot);
			zRoot.EndObject();
		}

		void cLog::ApplyLogTextWin(cmp::cTextWindow& zLogListener, const std::string& zLog)
		{
			zLogListener.SetText(zLog);
		}

		void cLog::ApplyLogOutStream(cmp::cOutStream& zLogListener, const std::string& zLog)
		{
			//TODO: fix this
			std::cout << zLog;
		}
	}
}