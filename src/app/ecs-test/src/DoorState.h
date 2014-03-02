#pragma once
#include <cstdint>
#include <ecs/ecs-config.h>


namespace pgn{
    namespace rl{
        namespace cmp{
            enum class eDoorState : std::int8_t {
                Open = 0,
                Closed = 1,
                Locked = 2
                };
                
            struct cDoorState
            {
                cDoorState(const eDoorState& arg0 = eDoorState(eDoorState::Closed)):
                        mState(arg0){}
                
                bool SerializeIn(const node_type&);
                void SerializeOut(node_type&, const std::string&) const;
                
                //!Door Status
                eDoorState mState;
                
            };
        
        }
    }
    DECL_SERIALIZE_INTERFACE( rl::cmp::eDoorState )
}
