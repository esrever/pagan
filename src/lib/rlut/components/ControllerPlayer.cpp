
#include "ControllerPlayer.h"

namespace pgn{
    namespace ecs{
        namespace cmp{
            //----------------------------------------------------------------------------------------------
            size_t cControllerPlayer::SerializeIn(const node_type& reader)
            {
                size_t ret = 0;
                ret += pgn::SerializeIn(reader,  "Bindings", mBindings);
                return ret;
            }
            
            //----------------------------------------------------------------------------------------------
            void cControllerPlayer::SerializeOut(node_type& writer) const
            {
                pgn::SerializeOut(writer,"Bindings", mBindings);
            }
        }
    }
}

