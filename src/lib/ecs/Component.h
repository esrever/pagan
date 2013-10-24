#pragma once

#include <typeinfo>
#include <typeindex>

#include <core/util/json_conversions.h>

#include "ComponentBase.h"

namespace pgn
{
	//! Templated class on the actual data type
	template<class T>
	class cComponent : public cComponentBase
	{
		public:
			typedef T datatype;
		public:			
			//! Get type index(for masks), statically
			static unsigned short StaticTypeIndex() {return msTypeIndex;}
			//! Get type index(for masks), virtually
			virtual unsigned short TypeIndex() const {return StaticTypeIndex();}

			//! Json conversions
			virtual void to_json(rapidjson::Value& zRoot) const;
			virtual bool from_json(const rapidjson::Value& zRoot);

			//! Object creator function
			static std::shared_ptr<cComponentBase> Create() { return std::shared_ptr<cComponentBase>(new cComponent<T>());}
			
		public:
			T mData;

		private:
			friend class cEntityMgr;
			static unsigned short msTypeIndex;
	};

	//------------------------------------------------------------------------
	template<class T>
	unsigned short cComponent<T>::msTypeIndex( 0xFFFF );

	//------------------------------------------------------------------------
	template<class T>
	void cComponent<T>::to_json(rapidjson::Value& zRoot) const
	{
		pgn::to_json< T >(mData, zRoot);
	}

	//------------------------------------------------------------------------
	template<class T>
	bool cComponent<T>::from_json(const rapidjson::Value& zRoot)
	{
		return pgn::from_json< T >(mData, zRoot);
	}
}