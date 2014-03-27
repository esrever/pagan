
#include "Creature.h"

namespace pgn{
    namespace ecs{
        namespace cmp{
            //----------------------------------------------------------------------------------------------
            size_t cCreature::SerializeIn(const node_type& reader)
            {
                size_t ret = 0;
                ret += pgn::SerializeIn(reader,  "CreatureData", mCreatureData);
                return ret;
            }
            
            //----------------------------------------------------------------------------------------------
            void cCreature::SerializeOut(node_type& writer) const
            {
                pgn::SerializeOut(writer,"CreatureData", mCreatureData);
            }
        }
    }
}

