#include "componentquery.h"

#include <algorithm>

namespace pgn
{
	namespace ecs
	{
		cComponentQuery::cComponentQuery()
			: INIT_EVT_MEMBER(cComponentQuery, EntityCreated),
			INIT_EVT_MEMBER(cComponentQuery, EntityDestroy),
			INIT_EVT_MEMBER(cComponentQuery, ComponentAdded)
		{

		}

		//-------------------------------------------------------------------------
		void cComponentQuery::OnEntityCreated(ecs::cEntityWithData ed)
		{
			bool ok = std::all_of(mRequiredComponentTypes.begin(),
							   mRequiredComponentTypes.end(),
							   [&](size_t i)->bool{return (ed->second.mSupportMask.at(i) == 1); });
			if (ok)
				mData.insert(ed);
		}

		//-------------------------------------------------------------------------
		void cComponentQuery::OnEntityDestroy(ecs::cEntityWithData ed)
		{
			bool ok = std::all_of(mRequiredComponentTypes.begin(),
				mRequiredComponentTypes.end(),
				[&](size_t i)->bool{return (ed->second.mSupportMask.at(i) == 1); });

			if (ok)
				mData.erase(ed);
		}

		//-------------------------------------------------------------------------
		void cComponentQuery::OnComponentAdded(ecs::cEntityWithData ed, unsigned short id)
		{
			bool ok = mRequiredComponentTypes.find(id) != mRequiredComponentTypes.end();
			if (ok)
				mData.insert(ed);
		}
	}
}