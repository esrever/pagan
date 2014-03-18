
#include "WorldData.h"

namespace pgn{
    namespace rl{
        namespace cmp{
            //----------------------------------------------------------------------------------------------
            size_t cWorldData::SerializeIn(const node_type& reader)
            {
                size_t ret = 0;
                ret = 1;
                return ret;
            }
            
            //----------------------------------------------------------------------------------------------
            void cWorldData::SerializeOut(node_type& writer) const
            {
                
            }
        }
    }
}

