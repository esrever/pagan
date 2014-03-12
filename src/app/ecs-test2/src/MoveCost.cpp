
#include "MoveCost.h"

namespace pgn{
    namespace rl{
        namespace cmp{
            //----------------------------------------------------------------------------------------------
            size_t cMoveCost::SerializeIn(const node_type& reader)
            {
                return pgn::SerializeIn(reader,"MoveCost", mMoveCost);
            }
            
            //----------------------------------------------------------------------------------------------
            void cMoveCost::SerializeOut(node_type& writer) const
            {
                pgn::SerializeOut(writer,"MoveCost", mMoveCost);
            }
        }
    }
    
    
}

