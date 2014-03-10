
#include "DoorState.h"

namespace pgn{
    namespace rl{
        namespace cmp{
            //----------------------------------------------------------------------------------------------
            bool cDoorState::SerializeIn(const node_type& reader)
            {
                pgn::SerializeIn(reader.child("State"), mState);
                return true;
            }
            
            //----------------------------------------------------------------------------------------------
            void cDoorState::SerializeOut(node_type& writer, const std::string& key) const
            {
                writer.append_attribute("value").set_value(key.c_str());
                pgn::SerializeOut(writer,"State", mState);
            }
        }
    }
    
    
    //----------------------------------------------------
    void SerializeOut(node_type& writer, const std::string& key, const rl::cmp::eDoorState & value)
    {
        pgn::SerializeOut(writer,key, int(value));
    }
    //----------------------------------------------------
    bool SerializeIn(const node_type& reader, rl::cmp::eDoorState & value)
    {
        int v;
        pgn::SerializeIn(reader, v);
        value = (rl::cmp::eDoorState)v;
        return true;
    }
    
}

