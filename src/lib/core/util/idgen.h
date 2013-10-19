#pragma once

#include <limits> 

namespace pgn
{
	template<class T>
	class cIdGen
	{
		public:
			cIdGen():mVal(0){}
			const T New() {return mVal++;}
			void Destroy(T val) {}
		private:
			T mVal;
	};
}