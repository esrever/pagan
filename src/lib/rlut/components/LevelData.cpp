
#include "LevelData.h"

namespace pgn{
    namespace rl{
        namespace cmp{
            //----------------------------------------------------------------------------------------------
            size_t cLevelData::SerializeIn(const node_type& reader)
            {
                size_t ret = 0;
                ret += pgn::SerializeIn(reader,  "Id", mId);
                return ret;
            }
            
            //----------------------------------------------------------------------------------------------
            void cLevelData::SerializeOut(node_type& writer) const
            {
                pgn::SerializeOut(writer,"Id", mId);
            }
        }
    }
}

