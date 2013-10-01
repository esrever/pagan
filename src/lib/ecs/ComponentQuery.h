#pragma once

#include <set>

#include "ecs_config.h"
#include "event.h"
#include "EntityComponents.h"

namespace pgn
{
	class cComponentQuery : public cEventReceiver<cComponentAddedEventData>,
							public cEventReceiver<cRemoveComponentEventData>,
							public cEventReceiver<cEntityCreatedEventData>,
							public cEventReceiver<cDestroyEntityEventData>
	{
	public:

		cComponentQuery(const component_mask_type& zMask);
		void Receive(const cComponentAddedEventData& zData);
		void Receive(const cRemoveComponentEventData& zData);
		void Receive( const cEntityCreatedEventData& zData);
		void Receive( const cDestroyEntityEventData& zData);
		const std::set<cEntityWptr>& Get() const {return mEntitiesWithComponents;}
	private:
		std::set<cEntityWptr> mEntitiesWithComponents;
	};
}