#pragma once
#include <cstdint>
#include <ecs/ecs-config.h>


namespace pgn{
    namespace rl{
        namespace cmp{
            
            struct cControllerPlayer
            {
                cControllerPlayer(const int& arg0 = int()):
                        mBindings(arg0){}
                
                bool SerializeIn(const node_type&);
                void SerializeOut(node_type&, const std::string&) const;
                
                //!Action key-bindings for player-controlled actors
                int mBindings;
                
            };
        
        }
    }
    
}
