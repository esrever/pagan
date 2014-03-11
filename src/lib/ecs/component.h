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

			virtual void SerializeOut(node_type& writer) const;
			virtual size_t SerializeIn(const node_type& reader);

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
	void cComponent<T>::SerializeOut(node_type& writer) const
	{
		mData.SerializeOut(writer);
	}

	//------------------------------------------------------------------------
	template<class T>
	size_t cComponent<T>::SerializeIn(const node_type& reader)
	{
		return mData.SerializeIn(reader);
	}
}