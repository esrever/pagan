#pragma once
#include <cstdint>
#include <ecs/ecs-config.h>
#include <map>
#include <set>
#include <core/util/glm_utils.h>
#include <core/container/Array2D.h>

namespace pgn{
    namespace rl{
        namespace cmp{
            
            struct cVisibility
            {
                cVisibility(const std::map< int, cArray2D<bool> >& arg0 = std::map< int, cArray2D<bool> >(),
                    const std::set<glm::ivec2>& arg1 = std::set<glm::ivec2>()):
                        mExplored(arg0),
                        mVisible(arg1){}
                
                size_t SerializeIn(const node_type&);
                void SerializeOut(node_type&) const;
                
                //!Explored flag for levels
                std::map< int, cArray2D<bool> > mExplored;
                
                //!Visible tiles
                std::set<glm::ivec2> mVisible;
                
            };
        
        }
    }
}
