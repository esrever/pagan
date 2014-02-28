#include "RegisterComponents.h"

#include <ecs/ecs.h>
#include <ecs/component.h>

// TODO: add all component includes
#include "cmptest.h"

namespace pgn
{
	void RegisterComponents(cECS& ecs)
	{
		ecs.RegisterComponent< cComponent<cCmpTest>>();
	}
}