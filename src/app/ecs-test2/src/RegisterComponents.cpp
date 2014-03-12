
#include "RegisterComponents.h"

#include <ecs/ecs.h>
#include <ecs/component.h>

// Add all component includes
#include "components.h"

namespace pgn {
    namespace rl {
        void RegisterComponents(cECS& ecs)
        {
            ecs.RegisterComponent<cmp::cMoveCost>();
            ecs.RegisterComponent<cmp::cTextureSet>();
            ecs.RegisterComponent<cmp::cStats>();
            ecs.RegisterComponent<cmp::cDoorState>();
        }
    }
}