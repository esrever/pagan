
#include "MapDiFi.h"

namespace pgn{
    namespace ecs{
        namespace cmp{
            //----------------------------------------------------------------------------------------------
            size_t cMapDiFi::SerializeIn(const node_type& reader)
            {
                size_t ret = 0;
                ret = 1;
                return ret;
            }
            
            //----------------------------------------------------------------------------------------------
            void cMapDiFi::SerializeOut(node_type& writer) const
            {
                
            }
        }
    }
}

