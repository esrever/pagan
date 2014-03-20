#pragma once
#include <cstdint>
#include <ecs/ecs-config.h>


namespace pgn{
    namespace ecs{
        namespace cmp{
            enum class eType : std::int8_t {
                Bg = 0,
                Fg = 1,
                Atmo = 2,
                Actor = 3
                };
                
            struct cTileType
            {
                cTileType(const eType& arg0 = eType(eType::Bg)):
                        mType(arg0){}
                
                size_t SerializeIn(const node_type&);
                void SerializeOut(node_type&) const;
                
                //!Tile type
                eType mType;
                
            };
        
        }
    }
}
