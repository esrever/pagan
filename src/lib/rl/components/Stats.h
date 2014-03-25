#pragma once
#include <cstdint>
#include <ecs/ecs-config.h>


namespace pgn{
    namespace ecs{
        namespace cmp{
            
            struct cStats
            {
                cStats(const int& arg0 = int(10),
                    const int& arg1 = int(10),
                    const int& arg2 = int(10)):
                        mStrength(arg0),
                        mDexterity(arg1),
                        mConstitution(arg2){}
                
                size_t SerializeIn(const node_type&);
                void SerializeOut(node_type&) const;
                
                //!Strength
                int mStrength;
                
                //!Dexterity
                int mDexterity;
                
                //!Constitution
                int mConstitution;
                
            };
        
        }
    }
}
