#pragma once
#include <cstdint>
#include <ecs/ecs-config.h>
#include <map>
#include <set>
#include <core/util/glm_utils.h>
#include <core/container/Array2D.h>

namespace pgn{
    namespace ecs{
        namespace cmp{
            
            struct cVisibility
            {
                cVisibility(const std::map< int, cArray2D<bool> >& arg0 = std::map< int, cArray2D<bool> >(),
                    const cArray2D<bool>& arg1 = cArray2D<bool>(),
                    const std::set<glm::ivec2>& arg2 = std::set<glm::ivec2>(),
                    const int& arg3 = int(1)):
                        mExplored(arg0),
                        mVisible(arg1),
                        mVisibleSet(arg2),
                        mLoS(arg3){}
                
                size_t SerializeIn(const node_type&);
                void SerializeOut(node_type&) const;
                
                //!Explored flag for levels
                std::map< int, cArray2D<bool> > mExplored;
                
                //!Visible flag for the current level
                cArray2D<bool> mVisible;
                
                //!Visible tiles
                std::set<glm::ivec2> mVisibleSet;
                
                //!Line of Sight
                int mLoS;
                
            };
        
        }
    }
}
