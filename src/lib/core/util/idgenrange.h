#pragma once

#include <boost/dynamic_bitset.hpp>

#include "idgen.h"

namespace pgn
{
	template<class T>
	class cIdGenBits : public cIdGen
	{
	public:
		virtual const T New();
		virtual const void Add(const T val);
		virtual void Destroy(T val);
	private:
		boost::dynamic_bitset<> mBits; 
	};

	//----------------------------------------------
	template<class T>
	const T cIdGenRanges::New()
	{
		
	}
}