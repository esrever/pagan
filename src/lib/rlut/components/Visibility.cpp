
#include "Visibility.h"

namespace pgn{
    namespace ecs{
        namespace cmp{
            //----------------------------------------------------------------------------------------------
            size_t cVisibility::SerializeIn(const node_type& reader)
            {
                size_t ret = 0;
                ret = 1;
                return ret;
            }
            
            //----------------------------------------------------------------------------------------------
            void cVisibility::SerializeOut(node_type& writer) const
            {
                
            }
        }
    }
}

