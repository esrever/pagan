#pragma once
#include <cstdint>
#include <ecs/ecs-config.h>


namespace pgn{
    namespace rl{
        namespace cmp{
            
            struct cMoveCost
            {
                cMoveCost(const float& arg0 = float(1)):
                        mMoveCost(arg0){}
                
				size_t SerializeIn(const node_type&);
				void SerializeOut(node_type&) const;
                
                //!Cost of moving horizontally or vertically to a tile
                float mMoveCost;
                
            };
        
        }
    }
    
}
