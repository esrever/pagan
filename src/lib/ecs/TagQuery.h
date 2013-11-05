#pragma once

#include <set>

#include "QueryBase.h"
#include "EntityComponents.h"

namespace pgn
{
	//! Stores entities with at least the specified components. 
	class cTagQuery : public cQueryBase
	{
	public:
		//! ctors
		cTagQuery(const std::string& zTag);
		cTagQuery(const std::vector<std::string>& zTags);

		//! Receive functions
		void OnEntityTagged( cEntity e, const std::string& zTag);
		void OnEntityUntag(cEntity e, const std::string& zTag);
		// TODO: add tagremove

		//! Access data
		const std::vector<std::string>& Tags() const {return mTags;}

	private:
		cEventHandler<cEntityTaggedEvent> mOnEntityTagged;
		cEventHandler<cEntityUntagEvent> mOnEntityUntag;
		cEventHandler<cEntityCreatedEvent> mOnEntityCreated;
		cEventHandler<cEntityDestroyEvent> mOnEntityDestroy;
		cEventHandler<cComponentAddedEvent> mOnComponentAdded;
		cEventHandler<cComponentRemoveEvent> mOnComponentRemove;
	private:
		void Init();
	private:
		std::vector<std::string> mTags;
	};
}
