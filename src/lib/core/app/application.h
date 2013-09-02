#pragma once

#include <map>
#include <string>

#include <core/util/cfgtree.h>
#include <core/util/logstream.h>


namespace pgn
{
	class cApplication
	{
		public: 
			cApplication(int argc, char ** argv);
			virtual ~cApplication();
			
			const cCfgTree& Config(const std::string& zCfgName = "") { return mConfigMap[zCfgName]; }
			cLogStream& LogStream(const std::string& zLogName = "") { return mLogMap[zLogName]; }

		private: 
			void InitCmdLine(int argc, char ** argv);
			void InitLoggers();
		private:
			std::map< std::string, cCfgTree> mConfigMap;
			std::map< std::string, cLogStream> mLogMap;
	};
}