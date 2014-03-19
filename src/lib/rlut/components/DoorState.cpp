
#include "DoorState.h"

namespace pgn{
    namespace ecs{
        namespace cmp{
            //----------------------------------------------------------------------------------------------
            size_t cDoorState::SerializeIn(const node_type& reader)
            {
                size_t ret = 0;
                ret += pgn::SerializeIn(reader,  "State", (int&)(mState));
                return ret;
            }
            
            //----------------------------------------------------------------------------------------------
            void cDoorState::SerializeOut(node_type& writer) const
            {
                pgn::SerializeOut(writer,"State", int(mState));
            }
        }
    }
}

