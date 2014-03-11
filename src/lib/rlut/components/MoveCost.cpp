
#include "MoveCost.h"

namespace pgn{
    namespace rl{
        namespace cmp{
            //----------------------------------------------------------------------------------------------
            size_t cMoveCost::SerializeIn(const node_type& reader)
            {
                size_t ret = 0;
                ret += pgn::SerializeIn(reader,  "MoveCost", mMoveCost);
                return ret;
            }
            
            //----------------------------------------------------------------------------------------------
            void cMoveCost::SerializeOut(node_type& writer) const
            {
                pgn::SerializeOut(writer,"MoveCost", mMoveCost);
            }
        }
    }
}

