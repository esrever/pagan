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
				auto ret = mData.insert(ed);
				if (mOnEntityAdded && ret.second)
					mOnEntityAdded(ed);
			}
		}

		//-------------------------------------------------------------------------
		void cComponentQuery::OnEntityDestroy(ecs::cEntityWithData ed)
		{
			if (Test(ed))
			{
				if (mOnEntityRemoved)
					mOnEntityRemoved(ed);
				mData.erase(ed);
			}
		}

		//-------------------------------------------------------------------------
		void cComponentQuery::OnComponentAdded(ecs::cEntityWithData ed, unsigned short id)
		{
			if (Test(ed))
			{
				auto ret = mData.insert(ed);
				if (mOnEntityAdded && ret.second)
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