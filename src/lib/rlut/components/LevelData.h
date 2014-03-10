#pragma once
#include <cstdint>
#include <ecs/ecs-config.h>
#include <core/container/Array2D.h>
#include <rlut/tile/tileinfo.h>

namespace pgn{
    namespace rl{
        namespace cmp{
            
            struct cLevelData
            {
                cLevelData(const int& arg0 = int(-1),
                    const cArray2D<cTileInfo>& arg1 = cArray2D<cTileInfo>()):
                        mId(arg0),
                        mMapData(arg1){}
                
                bool SerializeIn(const node_type&);
                void SerializeOut(node_type&, const std::string&) const;
                
                //!Level ID
                int mId;
                
                //!MapData
                cArray2D<cTileInfo> mMapData;
                
            };
        
        }
    }
    
}
