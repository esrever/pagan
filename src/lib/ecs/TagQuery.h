#pragma once

#include <set>

#include "Entity.h"
#include "QueryBase.h"

namespace pgn
{
	//! Stores entities with at least the specified components. 
	class cTagQuery : public cQueryBase,
					  public cEventReceiver<cEntityCreatedEventData>,
					  public cEventReceiver<cDestroyEntityEventData>,
					  public cEventReceiver<cEntityTaggedEventData>,
					  public cEventReceiver<cEntityUntagEventData>
	{
	public:
		//! ctors
		cTagQuery(const std::string& zTag);
		cTagQuery(const std::vector<std::string>& zTags);

		//! Receive functions
		void Receive( const cEntityCreatedEventData& zData);
		void Receive( const cDestroyEntityEventData& zData);
		void Receive( const cEntityTaggedEventData& zData);
		void Receive( const cEntityUntagEventData& zData);

		//! Name
		virtual const std::string ReceiverName() const {return "TagQuery";}

		//! Access data
		const std::vector<std::string>& Tags() const {return mTags;}

	private:
		void Init();
	private:
		std::vector<std::string> mTags;
	};
}
