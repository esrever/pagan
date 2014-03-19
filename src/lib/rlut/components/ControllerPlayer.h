#pragma once
#include <cstdint>
#include <ecs/ecs-config.h>
#include <map>
#include <ecs/ecs.h>

namespace pgn{
    namespace ecs{
        namespace cmp{
            
            struct cControllerPlayer
            {
                cControllerPlayer(const ecs::cActionBindings& arg0 = ecs::cActionBindings()):
                        mBindings(arg0){}
                
                size_t SerializeIn(const node_type&);
                void SerializeOut(node_type&) const;
                
                //!Action key-bindings for player-controlled actors
                ecs::cActionBindings mBindings;
                
            };
        
        }
    }
}
