#pragma once
#include <cstdint>
#include <ecs/ecs-config.h>
#include <rl/path/difi.h>

namespace pgn{
    namespace ecs{
        namespace cmp{
            
            struct cMapDiFi
            {
                cMapDiFi(const rl::cDiFi& arg0 = rl::cDiFi()):
                        mValue(arg0){}
                
                size_t SerializeIn(const node_type&);
                void SerializeOut(node_type&) const;
                
                //!DiFi to be used as a goal map for entities
                rl::cDiFi mValue;
                
            };
        
        }
    }
}
