
#include "LevelData.h"

namespace pgn{
    namespace rl{
        namespace cmp{
            //----------------------------------------------------------------------------------------------
            bool cLevelData::SerializeIn(const node_type& reader)
            {
                pgn::SerializeIn(reader.child("Id"), mId);
                return true;
            }
            
            //----------------------------------------------------------------------------------------------
            void cLevelData::SerializeOut(node_type& writer, const std::string& key) const
            {
                writer.append_attribute("value").set_value(key.c_str());
                pgn::SerializeOut(writer,"Id", mId);
            }
        }
    }
    
    
}

