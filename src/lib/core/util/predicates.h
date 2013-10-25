#pragma once

#include <boost/functional/hash.hpp>

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

	//------------------------------------------------------------------------------------
	template <typename Container> // we can make this generic for any container [1]
	struct container_hash {
		std::size_t operator()(Container const& c) const {
			return boost::hash_range(c.begin(), c.end());
		}
	};
}