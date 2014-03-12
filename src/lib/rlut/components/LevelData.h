#pragma once
#include <cstdint>
#include <ecs/ecs-config.h>
#include <rlut/tile/layout.h>

namespace pgn{
    namespace rl{
        namespace cmp{
            
            struct cLevelData
            {
                cLevelData(const int& arg0 = int(-1),
                    const cLayout& arg1 = cLayout()):
                        mId(arg0),
                        mLayout(arg1){}
                
                size_t SerializeIn(const node_type&);
                void SerializeOut(node_type&) const;
                
                //!Level ID
                int mId;
                
                //!Level layout
                cLayout mLayout;
                
            };
        
        }
    }
}
