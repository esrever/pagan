
#include "ControllerAI.h"

namespace pgn{
    namespace rl{
        namespace cmp{
            //----------------------------------------------------------------------------------------------
            size_t cControllerAI::SerializeIn(const node_type& reader)
            {
                size_t ret = 0;
                ret += pgn::SerializeIn(reader,  "Btree", mBtree);
                return ret;
            }
            
            //----------------------------------------------------------------------------------------------
            void cControllerAI::SerializeOut(node_type& writer) const
            {
                pgn::SerializeOut(writer,"Btree", mBtree);
            }
        }
    }
}

