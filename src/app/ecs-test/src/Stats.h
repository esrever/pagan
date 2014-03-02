#pragma once
#include <cstdint>
#include <ecs/ecs-config.h>


namespace pgn{
    namespace rl{
        namespace cmp{
            
            struct cStats
            {
                cStats(const float& arg0 = float(1),
                    const float& arg1 = float(1),
                    const float& arg2 = float(1)):
                        mDamage(arg0),
                        mHitPoints(arg1),
                        mSpeed(arg2){}
                
                bool SerializeIn(const node_type&);
                void SerializeOut(node_type&, const std::string&) const;
                
                //!Damage
                float mDamage;
                
                //!Max health
                float mHitPoints;
                
                //!Speed
                float mSpeed;
                
            };
        
        }
    }
    
}
