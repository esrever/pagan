
#include "Stats.h"

namespace pgn{
    namespace rl{
        namespace cmp{
            //----------------------------------------------------------------------------------------------
            size_t cStats::SerializeIn(const node_type& reader)
            {
				size_t ret = 0;
                ret += pgn::SerializeIn(reader,"Damage", mDamage);
				ret += pgn::SerializeIn(reader, "HitPoints", mHitPoints);
				ret += pgn::SerializeIn(reader, "Speed", mSpeed);
                return ret;
            }
            
            //----------------------------------------------------------------------------------------------
            void cStats::SerializeOut(node_type& writer) const
            {
                pgn::SerializeOut(writer,"Damage", mDamage);
                pgn::SerializeOut(writer,"HitPoints", mHitPoints);
                pgn::SerializeOut(writer,"Speed", mSpeed);
            }
        }
    }
    
    
}

