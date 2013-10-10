#pragma once

namespace pgn
{
	template<class T, class U>
	class cMapValueFinder
	{
		public:
			cMapValueFinder(const U& v):mRef(v){}
			bool operator () (typename const std::pair< T,  U>& v2) const {return mRef == v2.second;}
		private:
			const U& mRef;
	};
}