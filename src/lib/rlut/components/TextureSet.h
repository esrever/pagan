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
                cTextureSet(const std::vector< cSubTexture >& arg0 = std::vector< cSubTexture >(),
                    const int& arg1 = int(0)):
                        mSprites(arg0),
                        mIndex(arg1){}
                
                size_t SerializeIn(const node_type&);
                void SerializeOut(node_type&) const;
                
                //!Textures of a component
                std::vector< cSubTexture > mSprites;
                
                //!Index of active texture.
                int mIndex;
                
            };
        
        }
    }
}
