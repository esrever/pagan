
#include "TileInfo.h"

namespace pgn{
    namespace ecs{
        namespace cmp{
            //----------------------------------------------------------------------------------------------
            size_t cTileInfo::SerializeIn(const node_type& reader)
            {
                size_t ret = 0;
                ret += pgn::SerializeIn(reader,  "Type", (int&)(mType));
                return ret;
            }
            
            //----------------------------------------------------------------------------------------------
            void cTileInfo::SerializeOut(node_type& writer) const
            {
                pgn::SerializeOut(writer,"Type", int(mType));
            }
        }
    }
}

