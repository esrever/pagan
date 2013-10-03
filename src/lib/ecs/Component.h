#pragma once

#include <typeinfo>
#include <typeindex>

#include <core/util/json_conversions.h>

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
			
			virtual void to_json(rapidjson::Value& zRoot) const;
			virtual void from_json(const rapidjson::Value& zRoot);
			virtual std::string to_string() const;
		public:
			T mData;

		private:
			static unsigned short msTypeIndex;
	};

	//------------------------------------------------------------------------
	template<class T>
	unsigned short cComponent<T>::msTypeIndex( ECS.mEntityMgr.AddComponentType(typeid(T)));

	//------------------------------------------------------------------------
	template<class T>
	void cComponent<T>::to_json(rapidjson::Value& zRoot) const
	{
		pgn::to_json< typename cComponent<T> >(*this, zRoot);
	}

	//------------------------------------------------------------------------
	template<class T>
	void cComponent<T>::from_json(const rapidjson::Value& zRoot)
	{
		pgn::from_json< typename cComponent<T> >(*this, zRoot);
	}
	//------------------------------------------------------------------------
	template<class T>
	std::string cComponent<T>::to_string() const
	{
		rapidjson::Value root;
		this->to_json(root);
		return pgn::to_string(root);
	}
}