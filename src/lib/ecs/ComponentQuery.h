#pragma once

#include <set>

#include "ecs_config.h"
#include "event.h"
#include "EntityComponents.h"

namespace pgn
{
	//! Stores entities with at least the specified components. 
	class cComponentQuery : public cEventReceiver<cComponentAddedEventData>,
							public cEventReceiver<cRemoveComponentEventData>,
							public cEventReceiver<cEntityCreatedEventData>,
							public cEventReceiver<cDestroyEntityEventData>
	{
	public:
		virtual const std::string ReceiverName() const {return "asd";}
		cComponentQuery(const component_mask_type& zMask);
		void Receive(const cComponentAddedEventData& zData);
		void Receive(const cRemoveComponentEventData& zData);
		void Receive( const cEntityCreatedEventData& zData);
		void Receive( const cDestroyEntityEventData& zData);
		const std::set<cEntity>& Get() const {return mEntitiesWithComponents;}
		const component_mask_type& Mask() const {return mMask;}

		//! compare masks
		bool operator < (const cComponentQuery &rhs) const {return Mask().hash() < rhs.Mask().hash();}
	private:
		std::string mName;
		component_mask_type mMask;
		std::set<cEntity> mEntitiesWithComponents;
	};

	//-------------------------------------------------------------------------------
	bool is_subset(const component_mask_type& zAll, const component_mask_type& zSub);
}

	//! Global comparison operator 
	//inline bool std::less<pgn::cComponentQuery>::operator ()(const pgn::cComponentQuery &lhs ,const pgn::cComponentQuery &rhs) const {return lhs.Mask() < rhs.Mask();}