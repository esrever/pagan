#pragma once
#include <cstdint>
#include <ecs/ecs-config.h>
#include <rlut/ai/btree.h>

namespace pgn{
    namespace rl{
        namespace cmp{
            
            struct cControllerAI
            {
                cControllerAI(const bt::cBehaviorTree& arg0 = bt::cBehaviorTree()):
                        mBtree(arg0){}
                
                size_t SerializeIn(const node_type&);
                void SerializeOut(node_type&) const;
                
                //!Behavior Tree for AI-controlled actors
                bt::cBehaviorTree mBtree;
                
            };
        
        }
    }
}
