#pragma once

#include <core/util/Singleton.h>
#include <core/util/logstream.h>

#include "EntityMgr.h"
#include "SystemMgr.h"

#define ECS pgn::cSingleton<pgn::cECS>::Instance()
#define EMGR ECS.mEntityMgr
#define SMGR ECS.SystemMgr

namespace pgn
{
	class cECS
	{
		public: 
			void Init();
			void from_json(const rapidjson::Value& zRoot);
		public:
			// sub-managers
			cEntityMgr mEntityMgr;
			cSystemMgr mSystemMgr;
			cLogStream mLog;
	};
}