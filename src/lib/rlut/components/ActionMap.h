#pragma once
#include <cstdint>
#include <ecs/ecs-config.h>
#include <rlut/input/actionmap.h>

namespace pgn{
    namespace rl{
        namespace cmp{
            
            struct cActionMap
            {
                cActionMap(const rl::cActionMap& arg0 = rl::cActionMap()):
                        mActionMap(arg0){}
                
                size_t SerializeIn(const node_type&);
                void SerializeOut(node_type&) const;
                
                //!Action mapping class
                rl::cActionMap mActionMap;
                
            };
        
        }
    }
}
