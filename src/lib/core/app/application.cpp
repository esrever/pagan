#include "application.h"

#include <core/util/cmdopt.h>
#include <pystring.h>

namespace pgn
{
	//----------------------------------------------------------------------------
	cApplication::cApplication(int argc, char ** argv)
	{
		InitCmdLine(argc,argv);
		InitLoggers();
	}
	
	//----------------------------------------------------------------------------
	cApplication::~cApplication()
	{
		// free up resources
	}

	//----------------------------------------------------------------------------
	void cApplication::InitCmdLine(int argc, char ** argv)
	{
		// Use cmdopt to gather options
		cCmdOpt cmdopt;
		cmdopt.Init(argc, argv);


		std::vector<std::string> chunks;
		// Create the json tree
		for(auto itr : cmdopt.CfgData().mData)
		{
			std::string confname = "";
			if(pystring::startswith( itr.first, "jopt"))
			{
				pystring::split(itr.first,chunks,":",1);
				if(chunks.size() == 2)
					confname = chunks[1];
				const std::string jsonfname = itr.second[0];
				mConfigMap[confname] = cCfgTree(jsonfname);
			}
			else
			{
				mConfigMap[confname].mCfgData.Set(itr.first, itr.second);
			}
		}
	}

	//----------------------------------------------------------------------------
	void cApplication::InitLoggers()
	{
		cLogStream& logcl = mLogMap["OpenCL"];
		cLogStream& loggl = mLogMap["OpenGL"];
		cLogStream& logmain = mLogMap[""];
	}
}