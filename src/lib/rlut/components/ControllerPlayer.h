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
                cControllerPlayer(const std::map<std::string, ecs::cActionFunc>& arg0 = std::map<std::string, ecs::cActionFunc>()):
                        mBindings(arg0){}
                
                size_t SerializeIn(const node_type&);
                void SerializeOut(node_type&) const;
                
                //!Action key-bindings for player-controlled actors
                std::map<std::string, ecs::cActionFunc> mBindings;
                
            };
        
        }
    }
}
