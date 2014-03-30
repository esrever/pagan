
#include "SimpleVisibility.h"

namespace pgn{
    namespace ecs{
        namespace cmp{
            //----------------------------------------------------------------------------------------------
            size_t cSimpleVisibility::SerializeIn(const node_type& reader)
            {
                size_t ret = 0;
                ret += pgn::SerializeIn(reader,  "LoS", mLoS);
                return ret;
            }
            
            //----------------------------------------------------------------------------------------------
            void cSimpleVisibility::SerializeOut(node_type& writer) const
            {
                pgn::SerializeOut(writer,"LoS", mLoS);
            }
        }
    }
}

