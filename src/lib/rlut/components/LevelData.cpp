
#include "LevelData.h"

namespace pgn{
    namespace ecs{
        namespace cmp{
            //----------------------------------------------------------------------------------------------
            size_t cLevelData::SerializeIn(const node_type& reader)
            {
                size_t ret = 0;
                ret += pgn::SerializeIn(reader,  "Layout", mLayout);
                return ret;
            }
            
            //----------------------------------------------------------------------------------------------
            void cLevelData::SerializeOut(node_type& writer) const
            {
                pgn::SerializeOut(writer,"Layout", mLayout);
            }
        }
    }
}

