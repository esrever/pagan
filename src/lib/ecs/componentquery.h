#pragma once

#include <set>
#include <ecs/events.h>
#include <ecs/component.h>

namespace pgn
{
	namespace ecs
	{
		class cComponentQuery
		{
			public:
				cComponentQuery();
				void OnEntityCreated(ecs::cEntityWithData ed);
				void OnEntityDestroy(ecs::cEntityWithData ed);
				void OnComponentAdded(ecs::cEntityWithData ed, unsigned short);

				template<class T> 
				cComponentQuery& Require();
			private:
				DECL_EVT_MEMBER(EntityCreated);
				DECL_EVT_MEMBER(EntityDestroy);
				DECL_EVT_MEMBER(ComponentAdded);

				std::set<unsigned short>	   mRequiredComponentTypes;
				std::set<ecs::cEntityWithData> mData;
		};

		//--------------------------------------------
		template<class T>
		cComponentQuery& cComponentQuery::Require()
		{
			mRequiredComponentTypes.insert(cComponent<T>::StaticTypeIndex());
			return *this;
		}
	}
}