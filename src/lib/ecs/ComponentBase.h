#pragma once

#include <iostream>
#include <typeinfo>
#include <typeindex>

#include "ecs_config.h"

namespace pgn
{
	//! Class used for common storage.
	class cComponentBase
	{
	public:
		virtual ~cComponentBase(){}
		//! Get the component type index 
		virtual unsigned short TypeIndex() const {return 0xFFFF;}

		/* stream-style

			virtual std::ostream& operator << (std::ostream& o) const;
			virtual std::istream& operator >> (std::istream& i) const;

			config-style

			ReadJson, WriteJson( document& doc)
		*/
	protected:
		cComponentBase(){}
	};

	DECL_PTRTYPE(cComponentBase)

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
	unsigned short cComponent<T>::msTypeIndex( cEntityMgr::AddComponentType(std::typeid(T)));

}