
#include "Stats.h"

namespace pgn{
    namespace ecs{
        namespace cmp{
            //----------------------------------------------------------------------------------------------
            size_t cStats::SerializeIn(const node_type& reader)
            {
                size_t ret = 0;
                ret += pgn::SerializeIn(reader,  "Strength", mStrength);
                ret += pgn::SerializeIn(reader,  "Dexterity", mDexterity);
                ret += pgn::SerializeIn(reader,  "Constitution", mConstitution);
                return ret;
            }
            
            //----------------------------------------------------------------------------------------------
            void cStats::SerializeOut(node_type& writer) const
            {
                pgn::SerializeOut(writer,"Strength", mStrength);
                pgn::SerializeOut(writer,"Dexterity", mDexterity);
                pgn::SerializeOut(writer,"Constitution", mConstitution);
            }
        }
    }
}

