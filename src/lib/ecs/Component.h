#pragma once

#include <typeinfo>
#include <typeindex>

#include "Component.h"
#include "EntityMgr.h"

namespace pgn
{
	//! Templated class on the actual data type
	template<class T>
	class cComponent : public cComponentBase
	{
		public:
			// virtual void read_json() { read_json<T>(mData); } etc

			virtual unsigned short TypeIndex() const {return msTypeIndex;}
		public:
			T mData;

		private:
			static unsigned short msTypeIndex;
	};

	//------------------------------------------------------------------------
	template<class T>
	unsigned short cComponent<T>::msTypeIndex( ECS.AddComponentType(typeid(T)));
}