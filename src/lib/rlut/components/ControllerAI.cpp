
#include "ControllerAI.h"

namespace pgn{
    namespace rl{
        namespace cmp{
            //----------------------------------------------------------------------------------------------
            bool cControllerAI::SerializeIn(const node_type& reader)
            {
                pgn::SerializeIn(reader.child("Btree"), mBtree);
                return true;
            }
            
            //----------------------------------------------------------------------------------------------
            void cControllerAI::SerializeOut(node_type& writer, const std::string& key) const
            {
                writer.append_attribute("value").set_value(key.c_str());
                pgn::SerializeOut(writer,"Btree", mBtree);
            }
        }
    }
    
    
}

