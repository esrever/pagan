#pragma once

#include <set>

#include "Entity.h"

namespace pgn
{
	class cComponentQuery : public cEventReceiver<cComponentMaskModifiedEventData>
	{
	public:

		cComponentQuery(const component_mask_type& zMask);
		void Receive(const cComponentMaskModifiedEventData& zData);
	private:
		std::set<cEntityPtr> mEntitiesWithComponents;
	};
}