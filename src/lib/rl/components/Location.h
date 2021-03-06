#pragma once
#include <cstdint>
#include <ecs/ecs-config.h>
#include <glm/glm.hpp>

namespace pgn{
    namespace ecs{
        namespace cmp{
            
            struct cLocation
            {
                cLocation(const glm::ivec2& arg0 = glm::ivec2(),
                    const int& arg1 = int(-1)):
                        mPos(arg0),
                        mLevelId(arg1){}
                
                size_t SerializeIn(const node_type&);
                void SerializeOut(node_type&) const;
                
                //!Position
                glm::ivec2 mPos;
                
                //!Level ID
                int mLevelId;
                
            };
        
        }
    }
}
