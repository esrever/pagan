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
		pgn::cCmdOpt cmdopts;
		cmdopts.Init(argc,argv);
		pgn::cCfgTree& cfgtree = mConfigMap[""];
		cfgtree = cmdopts.CfgData();
		std::string jsonfname;
		if( cmdopts.CfgData().Get("jopt",jsonfname))
		{
			pgn::cCfgTree cfgjson(jsonfname);
			cfgtree.mCfgData.Merge(cfgjson.mCfgData);
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