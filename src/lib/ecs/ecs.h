#pragma once

#include <memory>
#include <string>
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

			void SetDataPath(const std::string& zPath) {mDataPath = zPath;}
			const std::string& GetDataPath() const {return mDataPath;}
			const std::string& GetErrorString() const {return mErrorString;}

		public:
			// sub-managers
			std::shared_ptr<cEntityMgr> mEntityMgr;
			std::shared_ptr<cSystemMgr> mSystemMgr;
			cLogStream mLog;
		private:
			std::string mDataPath;
			static const std::string mErrorString;
	};
}