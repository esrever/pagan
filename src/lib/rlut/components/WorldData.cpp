
#include "WorldData.h"

namespace pgn{
    namespace rl{
        namespace cmp{
            //----------------------------------------------------------------------------------------------
            size_t cWorldData::SerializeIn(const node_type& reader)
            {
                size_t ret = 0;
                ret += pgn::SerializeIn(reader,  "Pos", mPos);
                ret += pgn::SerializeIn(reader,  "LevelId", mLevelId);
                return ret;
            }
            
            //----------------------------------------------------------------------------------------------
            void cWorldData::SerializeOut(node_type& writer) const
            {
                pgn::SerializeOut(writer,"Pos", mPos);
                pgn::SerializeOut(writer,"LevelId", mLevelId);
            }
        }
    }
}

