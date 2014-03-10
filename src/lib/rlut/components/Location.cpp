
#include "Location.h"

namespace pgn{
    namespace rl{
        namespace cmp{
            //----------------------------------------------------------------------------------------------
            bool cLocation::SerializeIn(const node_type& reader)
            {
                pgn::SerializeIn(reader.child("Pos"), mPos);
                pgn::SerializeIn(reader.child("LevelId"), mLevelId);
                return true;
            }
            
            //----------------------------------------------------------------------------------------------
            void cLocation::SerializeOut(node_type& writer, const std::string& key) const
            {
                writer.append_attribute("value").set_value(key.c_str());
                pgn::SerializeOut(writer,"Pos", mPos);
                pgn::SerializeOut(writer,"LevelId", mLevelId);
            }
        }
    }
    
    
}

