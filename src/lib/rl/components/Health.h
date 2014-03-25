#pragma once
#include <cstdint>
#include <ecs/ecs-config.h>
#include <glm/glm.hpp>

namespace pgn{
    namespace ecs{
        namespace cmp{
            
            struct cHealth
            {
                cHealth(const glm::ivec2& arg0 = glm::ivec2(),
                    const int& arg1 = int()):
                        mHitDie(arg0),
                        mCurrentHealth(arg1){}
                
                size_t SerializeIn(const node_type&);
                void SerializeOut(node_type&) const;
                
                //!Per-level hit-die
                glm::ivec2 mHitDie;
                
                //!Current health
                int mCurrentHealth;
                
            };
        
        }
    }
}
