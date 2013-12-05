#pragma once

#include <bitset>
#include <map>

#include "ecs_config.h"
#include "Entity.h"
#include "ComponentBase.h"
#include "Component.h"

namespace pgn
{
	class cEntityComponents
	{
	public:
		typedef std::vector<cComponentBaseSptr> ComponentSet;
	public:
		//! ctors
		cEntityComponents();
		
		//! Component management
		void AddComponent(cComponentBaseSptr zCompo);
		void RemoveComponent(unsigned short zTypeId);

		//! Accessors
		const component_mask_type& Mask() const {return mMask;}
		const ComponentSet& Components() const {return mComponents;}

		template<class T>
		void GetComponent(std::shared_ptr<T >& zPtr) const;

		template<class T>
		bool HasComponent() const;

	private:
		ComponentSet mComponents;
		component_mask_type mMask;
	};

	//---------------------------------------------------------------------------
	template<class T>
	inline void cEntityComponents::GetComponent(std::shared_ptr<T>& zPtr) const
	{
		zPtr = std::dynamic_pointer_cast< T >( mComponents.at(T::StaticTypeIndex()));
	}

	template<class T>
	inline bool cEntityComponents::HasComponent() const
	{
		return Mask().at(T::StaticTypeIndex());
	}

	//---------------------------------------------------------------------------
	template<>
	void to_json<cEntityComponents>(const cEntityComponents& zObj, JsonWriter& writer);
}
