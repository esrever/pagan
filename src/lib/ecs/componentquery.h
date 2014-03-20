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
				enum class ePolicy
				{
					All,
					Any
				};
				cComponentQuery(ePolicy p = ePolicy::All);
				void OnEntityCreated(ecs::cEntityWithData ed);
				void OnEntityDestroy(ecs::cEntityWithData ed);
				void OnComponentAdded(ecs::cEntityWithData ed, unsigned short);

				const std::set<ecs::cEntityWithData>& Data() const { return mData; }

				template<class T> 
				cComponentQuery& Require();

				void SetOnEntityAdded(std::function<void(ecs::cEntityWithData)> f) { mOnEntityAdded = f; }
				void SetOnEntityRemoved(std::function<void(ecs::cEntityWithData)> f) { mOnEntityRemoved = f; }
			private:
				bool Test(ecs::cEntityWithData) const;
			private:
				DECL_EVT_MEMBER(EntityCreated);
				DECL_EVT_MEMBER(EntityDestroy);
				DECL_EVT_MEMBER(ComponentAdded);

				ePolicy						   mPolicy;
				std::set<unsigned short>	   mRequiredComponentTypes;
				std::set<ecs::cEntityWithData> mData;
				std::function<void(ecs::cEntityWithData)>	mOnEntityAdded;
				std::function<void(ecs::cEntityWithData)>	mOnEntityRemoved;
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