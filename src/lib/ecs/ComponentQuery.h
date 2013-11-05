#pragma once

#include "QueryBase.h"
#include "EntityComponents.h"


namespace pgn
{
	//! Stores entities with at least the specified components. 
	class cComponentQuery : public cQueryBase
	{
	public:
		//! ctors
		cComponentQuery(const component_mask_type& zMask);

		//! Receive functions
		void OnComponentAdded(cEntityWithComponents ec, unsigned short cid);
		void OnComponentRemove(cEntityWithComponents ec, unsigned short cid);
		void OnEntityCreated(cEntity e);
		void OnEntityDestroy(cEntity e);

		//! Access data
		const component_mask_type& Mask() const {return mMask;}

	private:
		cEventHandler<cEntityCreatedEvent> mOnEntityCreated;
		cEventHandler<cEntityDestroyEvent> mOnEntityDestroy;
		cEventHandler<cComponentAddedEvent> mOnComponentAdded;
		cEventHandler<cComponentRemoveEvent> mOnComponentRemove;
	private:
		component_mask_type mMask;
	};

	//-------------------------------------------------------------------------------
	bool is_subset(const component_mask_type& zAll, const component_mask_type& zSub);
}