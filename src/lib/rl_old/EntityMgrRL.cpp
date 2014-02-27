#include "EntityMgrRL.h"
#include "components/components.h"

namespace pgn
{
	void cEntityMgrRL::RegisterComponentTypes()
	{
		RegisterAllComponents(*this);
	}
}