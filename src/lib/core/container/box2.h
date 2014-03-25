#pragma once

#include <glm/glm.hpp>

#include <core/util/string.h>
#include <core/serialize/serialize.h>

namespace pgn
{
	template<class T>
	struct cBox2
	{
		typedef T data_type;
		typedef glm::detail::tvec2<data_type> vec_type;

		cBox2() :mStart(0, 0), mDims(0,0){}
		cBox2(const vec_type& dims) :mStart(0, 0), mDims(dims){}
		cBox2(const vec_type& start, const vec_type& dims) :mStart(start), mDims(dims){}

		bool operator == (const cBox2<T>& v) const
		{
			return memcmp(this, &v, sizeof(cBox2)) == 0;
		}
		
		vec_type mStart;
		vec_type mDims;
	};

	typedef cBox2<int> cBox2i;
	typedef cBox2<float> cBox2f;

	//----------------------------------------------------------------------
	template<class T>
	void SerializeOut(node_type& writer, const cBox2<T>& value)
	{
		SerializeOut(writer, "x", value.mStart.x);
		SerializeOut(writer, "y", value.mStart.y);
		SerializeOut(writer, "w", value.mDims.x);
		SerializeOut(writer, "h", value.mDims.y);
	}

	//----------------------------------------------------------------------
	template<class T>
	size_t SerializeIn(const node_type& reader, cBox2<T>& value)
	{
		size_t ret = 0;
		ret += SerializeIn(reader, "x", value.mStart.x);
		ret += SerializeIn(reader, "y", value.mStart.y);
		ret += SerializeIn(reader, "w", value.mDims.x);
		ret += SerializeIn(reader, "h", value.mDims.y);
		return ret;
	}
}

namespace std
{
	template < class T>
	inline const string to_string(const pgn::cBox2<T>& v)
	{
		std::ostringstream oss;
		oss << "{" << v.mStart.x << "," << v.mStart.y << "," << v.mDims.x << "," << v.mDims.y << "}";
		return oss.str();
	}

	template<class T>
	inline istream& operator>> (istream &in, pgn::cBox2<T>& box)
	{
		in >> box.mStart.x >> box.mStart.y >> box.mDims.x >> box.mDims.y;
	}
}