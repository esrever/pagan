
#include "Health.h"

namespace pgn{
    namespace ecs{
        namespace cmp{
            //----------------------------------------------------------------------------------------------
            size_t cHealth::SerializeIn(const node_type& reader)
            {
                size_t ret = 0;
                ret += pgn::SerializeIn(reader,  "HitDie", mHitDie);
                return ret;
            }
            
            //----------------------------------------------------------------------------------------------
            void cHealth::SerializeOut(node_type& writer) const
            {
                pgn::SerializeOut(writer,"HitDie", mHitDie);
            }
        }
    }
}

