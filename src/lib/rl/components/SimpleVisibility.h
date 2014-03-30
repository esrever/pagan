#pragma once
#include <cstdint>
#include <ecs/ecs-config.h>


namespace pgn{
    namespace ecs{
        namespace cmp{
            
            struct cSimpleVisibility
            {
                cSimpleVisibility(const int& arg0 = int(1)):
                        mLoS(arg0){}
                
                size_t SerializeIn(const node_type&);
                void SerializeOut(node_type&) const;
                
                //!Line of Sight
                int mLoS;
                
            };
        
        }
    }
}
