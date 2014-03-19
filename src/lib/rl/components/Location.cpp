
#include "Location.h"

namespace pgn{
    namespace ecs{
        namespace cmp{
            //----------------------------------------------------------------------------------------------
            size_t cLocation::SerializeIn(const node_type& reader)
            {
                size_t ret = 0;
                ret += pgn::SerializeIn(reader,  "Pos", mPos);
                ret += pgn::SerializeIn(reader,  "LevelId", mLevelId);
                return ret;
            }
            
            //----------------------------------------------------------------------------------------------
            void cLocation::SerializeOut(node_type& writer) const
            {
                pgn::SerializeOut(writer,"Pos", mPos);
                pgn::SerializeOut(writer,"LevelId", mLevelId);
            }
        }
    }
}

