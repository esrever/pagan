#pragma once
#include <cstdint>
#include <ecs/ecs-config.h>
#include <core/container/Array2D.h>

namespace pgn{
    namespace rl{
        namespace cmp{
            
            struct cLevelData
            {
                cLevelData(const int& arg0 = int(-1),
                    const cArray2D<int>& arg1 = cArray2D<int>()):
                        mId(arg0),
                        mMapData(arg1){}
                
                bool SerializeIn(const node_type&);
                void SerializeOut(node_type&, const std::string&) const;
                
                //!Level ID
                int mId;
                
                //!MapData
                cArray2D<int> mMapData;
                
            };
        
        }
    }
    
}
