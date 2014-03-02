
#include "Stats.h"

namespace pgn{
    namespace rl{
        namespace cmp{
            //----------------------------------------------------------------------------------------------
            bool cStats::SerializeIn(const node_type& reader)
            {
                pgn::SerializeIn(reader.child("Damage"), mDamage);
                pgn::SerializeIn(reader.child("HitPoints"), mHitPoints);
                pgn::SerializeIn(reader.child("Speed"), mSpeed);
                return true;
            }
            
            //----------------------------------------------------------------------------------------------
            void cStats::SerializeOut(node_type& writer, const std::string& key) const
            {
                writer.append_attribute("value").set_value(key.c_str());
                pgn::SerializeOut(writer,"Damage", mDamage);
                pgn::SerializeOut(writer,"HitPoints", mHitPoints);
                pgn::SerializeOut(writer,"Speed", mSpeed);
            }
        }
    }
    
    
}

