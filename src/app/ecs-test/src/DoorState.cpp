
#include "DoorState.h"

namespace pgn{
    namespace rl{
        namespace cmp{
            //----------------------------------------------------------------------------------------------
            size_t cDoorState::SerializeIn(const node_type& reader)
            {
                return pgn::SerializeIn(reader,"State", (int&)(mState));
            }
            
            //----------------------------------------------------------------------------------------------
            void cDoorState::SerializeOut(node_type& writer) const
            {
                pgn::SerializeOut(writer,"State", int(mState));
            }
        }
    }
    
    
    //----------------------------------------------------
    void SerializeOut(node_type& writer, const ecs::cmp::eDoorState & value)
    {
        pgn::SerializeOut(writer, int(value));
    }
    //----------------------------------------------------
    size_t SerializeIn(const node_type& reader, ecs::cmp::eDoorState & value)
    {
		size_t ret = 0;
        int v;
        ret += pgn::SerializeIn(reader, v);
        value = (ecs::cmp::eDoorState)v;
        return ret;
    }
    
}

