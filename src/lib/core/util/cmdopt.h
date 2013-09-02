#pragma once

#include "cfgdata.h"

/*
	DESCRIPTION:
		Fetch the command line argc/argv and turn it into a cCfgTree

		Format:
			a=b					// single key-value
			c=d,e,f				// key-value vector
*/

namespace pgn
{
	class cCfgTree;

    class cCmdOpt
    {
        public:            
            void Init(int argc, char ** argv);
            const cCfgData& CfgData() const        {  return mCfgData;    }
        
        private:
            cCfgData mCfgData;
    };
}