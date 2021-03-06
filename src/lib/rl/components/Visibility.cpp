
#include "Visibility.h"

namespace pgn{
    namespace ecs{
        namespace cmp{
            //----------------------------------------------------------------------------------------------
            size_t cVisibility::SerializeIn(const node_type& reader)
            {
                size_t ret = 0;
                ret += pgn::SerializeIn(reader,  "LoS", mLoS);
                return ret;
            }
            
            //----------------------------------------------------------------------------------------------
            void cVisibility::SerializeOut(node_type& writer) const
            {
                pgn::SerializeOut(writer,"LoS", mLoS);
            }
        }
    }
}

