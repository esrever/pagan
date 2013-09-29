#pragma once

#include <iostream>
#include <memory>
#include <typeinfo>
#include <typeindex>

namespace pgn
{
	class cComponentBase
	{
	public:
		virtual ~cComponentBase(){}

		virtual unsigned short BitId() const {return 0xFFFF;}

		/* stream-style

			virtual std::ostream& operator << (std::ostream& o) const;
			virtual std::istream& operator >> (std::istream& i) const;

			config-style

			ReadJson, WriteJson( document& doc)
		*/
	protected:
		cComponentBase(){}
	};

	typedef std::weak_ptr<cComponentBase> cComponentPtr;
	typedef std::shared_ptr<cComponentBase> cComponentSptr;

	template<class T>
	class cComponent : public cComponentBase
	{
		public:
			// virtual void read_json() { read_json<T>(mData); } etc

			virtual unsigned short BitId() const {return msBitId;}
		public:
			T mData;

		private:
			static unsigned short msBitId;
	};

	//------------------------------------------------------------------------
	template<class T>
	unsigned short cComponent<T>::msBitId( cEntityMgr::AddComponentType(std::typeid(T)));

}