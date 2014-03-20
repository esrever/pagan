
#include "TileType.h"

namespace pgn{
    namespace ecs{
        namespace cmp{
            //----------------------------------------------------------------------------------------------
            size_t cTileType::SerializeIn(const node_type& reader)
            {
                size_t ret = 0;
                ret += pgn::SerializeIn(reader,  "Type", (int&)(mType));
                return ret;
            }
            
            //----------------------------------------------------------------------------------------------
            void cTileType::SerializeOut(node_type& writer) const
            {
                pgn::SerializeOut(writer,"Type", int(mType));
            }
        }
    }
}

