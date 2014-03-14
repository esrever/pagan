
#include "ActionMap.h"

namespace pgn{
    namespace rl{
        namespace cmp{
            //----------------------------------------------------------------------------------------------
            size_t cActionMap::SerializeIn(const node_type& reader)
            {
                size_t ret = 0;
                ret += pgn::SerializeIn(reader,  "ActionMap", mActionMap);
                return ret;
            }
            
            //----------------------------------------------------------------------------------------------
            void cActionMap::SerializeOut(node_type& writer) const
            {
                pgn::SerializeOut(writer,"ActionMap", mActionMap);
            }
        }
    }
}

