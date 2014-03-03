#pragma once
#include <cstdint>
#include <ecs/ecs-config.h>
#include <vector>
#include <core/texture/texture.h>

namespace pgn{
    namespace rl{
        namespace cmp{
            
            struct cTextureSet
            {
                cTextureSet(const std::vector< cSubTexture >& arg0 = std::vector< cSubTexture >()):
                        mSprites(arg0){}
                
                bool SerializeIn(const node_type&);
                void SerializeOut(node_type&, const std::string&) const;
                
                //!Textures of a component
                std::vector< cSubTexture > mSprites;
                
            };
        
        }
    }
    
}
