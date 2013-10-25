#pragma once

#include "QueryBase.h"
#include "EntityComponents.h"


namespace pgn
{
	//! Stores entities with at least the specified components. 
	class cComponentQuery : public cQueryBase,
							public cEventReceiver<cComponentAddedEventData>,
							public cEventReceiver<cRemoveComponentEventData>,
							public cEventReceiver<cEntityCreatedEventData>,
							public cEventReceiver<cDestroyEntityEventData>
	{
	public:
		//! ctors
		cComponentQuery(const component_mask_type& zMask);

		//! Receive functions
		void Receive(const cComponentAddedEventData& zData);
		void Receive(const cRemoveComponentEventData& zData);
		void Receive( const cEntityCreatedEventData& zData);
		void Receive( const cDestroyEntityEventData& zData);

		//! Name
		virtual const std::string ReceiverName() const {return "ComponentQuery";}

		//! Access data
		const component_mask_type& Mask() const {return mMask;}

	private:
		component_mask_type mMask;
	};

	//-------------------------------------------------------------------------------
	bool is_subset(const component_mask_type& zAll, const component_mask_type& zSub);
}