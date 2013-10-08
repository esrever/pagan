#pragma once

#include <memory>
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
			void Destroy();
			void from_json(const rapidjson::Value& zRoot);
		public:
			// sub-managers
			std::shared_ptr<cEntityMgr> mEntityMgr;
			std::shared_ptr<cSystemMgr> mSystemMgr;
			cLogStream mLog;
	};
}