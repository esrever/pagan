#pragma once
#include <cstdint>
#include <ecs/ecs-config.h>
#include <map>
#include <ecs/ecs.h>

namespace pgn{
    namespace rl{
        namespace cmp{
            
            struct cWorldData
            {
                cWorldData(const std::map<int, ecs::cEntityWithData>& arg0 = std::map<int, ecs::cEntityWithData>()):
                        mLevelMap(arg0){}
                
                size_t SerializeIn(const node_type&);
                void SerializeOut(node_type&) const;
                
                //!LevelEntities
                std::map<int, ecs::cEntityWithData> mLevelMap;
                
            };
        
        }
    }
}
