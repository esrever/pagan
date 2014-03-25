#pragma once

#include <glm/glm.hpp>

namespace pgn
{
	template<class T>
	struct cBox2
	{
		typedef T data_type;
		typedef glm::tvec2<data_type> vec_type;

		cBox2() :mStart(0, 0), mDims(0,0){}
		cBox2(const vec_type& dims) :mStart(0, 0), mDims(dims){}
		cBox2(const vec_type& start, const vec_type& dims) :mStart(start), mDims(dims){}
		
		vec_type mStart;
		vec_type mDims;
	};

	typedef cBox2<int> cBox2i;
	typedef cBox2<float> cBox2f;
}