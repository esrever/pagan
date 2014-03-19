#pragma once
#include <cstdint>
#include <ecs/ecs-config.h>
#include <rlut/tile/layout.h>

namespace pgn{
    namespace ecs{
        namespace cmp{
            
            struct cLevelData
            {
                cLevelData(const rl::cLayout& arg0 = rl::cLayout()):
                        mLayout(arg0){}
                
                size_t SerializeIn(const node_type&);
                void SerializeOut(node_type&) const;
                
                //!Level layout
                rl::cLayout mLayout;
                
            };
        
        }
    }
}
