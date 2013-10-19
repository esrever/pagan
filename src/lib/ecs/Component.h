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
			// virtual void read_json() { read_json<T>(mData); } etc

			static unsigned short StaticTypeIndex() {return msTypeIndex;}
			virtual unsigned short TypeIndex() const {StaticTypeIndex();}

			virtual void to_json(rapidjson::Value& zRoot) const;
			virtual void from_json(const rapidjson::Value& zRoot);
			//virtual std::string to_string() const;
			
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
	void cComponent<T>::from_json(const rapidjson::Value& zRoot)
	{
		pgn::from_json< T >(mData, zRoot);
	}

}