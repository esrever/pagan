#pragma once
#include <cstdint>
#include <ecs/ecs-config.h>
#include <vector>
#include <rlut/sprite/sprite.h>

namespace pgn{
    namespace rl{
        namespace cmp{
            
            struct cTextureSet
            {
                cTextureSet(const std::vector< rlut::cSprite >& arg0 = std::vector< rlut::cSprite >()):
                        mSprites(arg0){}
                
                bool SerializeIn(const node_type&);
                void SerializeOut(node_type&, const std::string&) const;
                
                //!Textures of a component
                std::vector< rlut::cSprite > mSprites;
                
            };
        
        }
    }
    
}
