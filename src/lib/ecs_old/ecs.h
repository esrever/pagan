#pragma once

#include <memory>
#include <string>
#include <core/util/Singleton.h>
#include <core/util/logstream.h>

#include "EntityMgr.h"
#include "SystemMgr.h"
#include "ActionMgr.h"

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
			bool from_json(const rapidjson::Value& zRoot);

			void SetDataPath(const std::string& zPath) {mDataPath = zPath;}
			const std::string& GetDataPath() const {return mDataPath;}
			const std::string& GetErrorString() const {return mErrorString;}

		public:
			//! sub-managers
			std::shared_ptr<cEntityMgr> mEntityMgr;
			std::shared_ptr<cSystemMgr> mSystemMgr;
			std::shared_ptr<cActionMgr> mActionMgr;
			std::shared_ptr<cLogBase>	mLog;
		private:
			std::string mDataPath;
			static const std::string mErrorString;
	};

	template<>
	void to_json(const cECS& zObj, JsonWriter& zRoot);
}