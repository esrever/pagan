#pragma once

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
			cComponent(const T& data = T()):mData(data){}
			//! Get type index(for masks), statically
			static unsigned short StaticTypeIndex() {return msTypeIndex;}
			//! Get type index(for masks), virtually
			virtual unsigned short TypeIndex() const {return StaticTypeIndex();}

			virtual void SerializeOut(node_type& writer, const std::string& key) const;
			virtual bool SerializeIn(const node_type& reader);

			//! Object creator function
			static std::shared_ptr<cComponentBase> Create() { return std::shared_ptr<cComponentBase>(new cComponent<T>());}
			
		public:
			T mData;

		private:
			friend class cECS;
			static unsigned short msTypeIndex;
	};

	//------------------------------------------------------------------------
	template<class T>
	unsigned short cComponent<T>::msTypeIndex( 0xFFFF );

	//------------------------------------------------------------------------
	template<class T>
	void cComponent<T>::SerializeOut(node_type& writer, const std::string& key) const
	{
		auto& ecs = ECS();
		const std::string& tname = ecs.ComponentTypeNames().at(TypeIndex());
		auto& child = writer.append_child(key.c_str());
		mData.SerializeOut(child, tname);
	}

	//------------------------------------------------------------------------
	template<class T>
	bool cComponent<T>::SerializeIn(const node_type& reader)
	{
		return mData.SerializeIn(reader);
	}
}