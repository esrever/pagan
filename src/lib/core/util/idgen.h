#pragma once

#include <boost/dynamic_bitset.hpp>

namespace pgn
{
	template<class T>
	class cIdGen
	{
		public:
			static const unsigned msDefaultMaxIds = 1024;
			cIdGen():mBits(msDefaultMaxIds){}
			const T New();
			const void Add(const T val);
			void Destroy(T val);
		private:
			boost::dynamic_bitset<> mBits; 
	};	

	//-----------------------------------------
	template<class T>
	const T cIdGen<T>::New()
	{
		auto pos = (~mBits).find_first();
		if (pos == boost::dynamic_bitset<>::npos)
		{
			pos = mBits.size();
			mBits.push_back(true);
		}
		else
		{
			assert(!mBits.test(pos));
			mBits.flip(pos);
		}
		return T(pos);
	}
	//-----------------------------------------
	template<class T>
	const void cIdGen<T>::Add(const T val)
	{
		assert(!mBits.test(val));
		mBits.flip(val);
	}
	//-----------------------------------------
	template<class T>
	void cIdGen<T>::Destroy(T val)
	{
		assert(mBits.test(val));
		mBits.flip(val);
	}
}