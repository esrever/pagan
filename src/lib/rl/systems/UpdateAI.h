#pragma once

#include <map>
#include <string>

#include <core/event/Event.h>
#include <ecs/system.h>
#include <ecs/ecs.h>
#include <ecs/componentquery.h>

namespace pgn
{
	namespace bt
	{
		class cBehavior;
	}
	typedef std::map<std::string, bt::cBehavior *> cBehaviorDb;

	namespace ecs
	{
		namespace sys
		{
			class cUpdateAI : public cBase
			{
				public:
					cUpdateAI();

					//! Runs a tick() for the given entity
					bool operator()(ecs::cEntityWithData ed);

					//! Loads all the supported behaviors
					void LoadBehaviorDb(const char * fname);

					bt::cBehavior * GetBehavior(const std::string& name) const;
				
				private:
					//! Initializes Btree data of entity
					void InitializeBt(ecs::cEntityWithData ed);
				private:
					cComponentQuery mAIQuery;
					cBehaviorDb mBehaviorDb;
			};
		}
	}
}