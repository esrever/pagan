
#include "ControllerPlayer.h"

namespace pgn{
    namespace rl{
        namespace cmp{
            //----------------------------------------------------------------------------------------------
            bool cControllerPlayer::SerializeIn(const node_type& reader)
            {
                pgn::SerializeIn(reader.child("Bindings"), mBindings);
                return true;
            }
            
            //----------------------------------------------------------------------------------------------
            void cControllerPlayer::SerializeOut(node_type& writer, const std::string& key) const
            {
                writer.append_attribute("value").set_value(key.c_str());
                pgn::SerializeOut(writer,"Bindings", mBindings);
            }
        }
    }
    
    
}

