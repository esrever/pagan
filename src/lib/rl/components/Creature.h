#pragma once
#include <cstdint>
#include <ecs/ecs-config.h>
#include <rl/game/creature.h>

namespace pgn{
    namespace ecs{
        namespace cmp{
            
            struct cCreature
            {
                cCreature(const rl::cCreature& arg0 = rl::cCreature()):
                        mCreatureData(arg0){}
                
                size_t SerializeIn(const node_type&);
                void SerializeOut(node_type&) const;
                
                //!Creature data
                rl::cCreature mCreatureData;
                
            };
        
        }
    }
}
