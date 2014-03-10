
#include "MoveCost.h"

namespace pgn{
    namespace rl{
        namespace cmp{
            //----------------------------------------------------------------------------------------------
            bool cMoveCost::SerializeIn(const node_type& reader)
            {
                pgn::SerializeIn(reader.child("MoveCost"), mMoveCost);
                return true;
            }
            
            //----------------------------------------------------------------------------------------------
            void cMoveCost::SerializeOut(node_type& writer, const std::string& key) const
            {
                writer.append_attribute("value").set_value(key.c_str());
                pgn::SerializeOut(writer,"MoveCost", mMoveCost);
            }
        }
    }
    
    
}

