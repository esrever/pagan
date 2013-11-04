#pragma once

#include <set>

#include "Entity.h"
#include "QueryBase.h"

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

		//! Name
		virtual const std::string ReceiverName() const {return "TagQuery";}

		//! Access data
		const std::vector<std::string>& Tags() const {return mTags;}

	private:
		cEventHandler<cEntityTaggedEvent> mOnEntityTagged;
		cEventHandler<cEntityUntagEvent> mOnEntityUntag;
	private:
		void Init();
	private:
		std::vector<std::string> mTags;
	};
}
