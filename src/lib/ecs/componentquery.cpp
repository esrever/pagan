#include "componentquery.h"

#include <algorithm>

namespace pgn
{
	namespace ecs
	{
		cComponentQuery::cComponentQuery(ePolicy p)
			:mPolicy(p),
			INIT_EVT_MEMBER(cComponentQuery, EntityCreated),
			INIT_EVT_MEMBER(cComponentQuery, EntityDestroy),
			INIT_EVT_MEMBER(cComponentQuery, ComponentAdded)
		{

		}

		//-------------------------------------------------------------------------
		void cComponentQuery::OnEntityCreated(ecs::cEntityWithData ed)
		{
			if (Test(ed))
			{
				mData.insert(ed);
				if (mOnEntityAdded)
					mOnEntityAdded(ed);
			}
		}

		//-------------------------------------------------------------------------
		void cComponentQuery::OnEntityDestroy(ecs::cEntityWithData ed)
		{
			if (Test(ed))
			{
				mData.erase(ed);
				if (mOnEntityRemoved)
					mOnEntityRemoved(ed);
			}
		}

		//-------------------------------------------------------------------------
		void cComponentQuery::OnComponentAdded(ecs::cEntityWithData ed, unsigned short id)
		{
			if (Test(ed))
			{
				mData.insert(ed);
				if (mOnEntityAdded)
					mOnEntityAdded(ed);
			}
		}

		//-------------------------------------------------------------------------
		bool cComponentQuery::Test(ecs::cEntityWithData ed) const
		{
			bool ok = mPolicy == ePolicy::All ? std::all_of(mRequiredComponentTypes.begin(),
				mRequiredComponentTypes.end(),
				[&](size_t i)->bool{return (ed->second.mSupportMask.at(i) == 1); })
				: std::any_of(mRequiredComponentTypes.begin(),
				mRequiredComponentTypes.end(),
				[&](size_t i)->bool{return (ed->second.mSupportMask.at(i) == 1); });
			return ok;
		}
	}
}