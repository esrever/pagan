#pragma once

#include <core/util/xml_conversions.h>
#include <oxygine-framework.h>

namespace pgn
{
#define OVT( T , N ) oxygine::VectorT##N##< T >
#define XML_OXYVEC( T, N ) \
	template <>\
		inline void to_xml< OVT( T , N ) >(const OVT( T , N ) & zObj, pugi::xml_node& zRoot){\
		to_xml< T, N >(&zObj.x, zRoot);}\
	template <>\
	inline void from_xml< OVT(T, N) >( OVT(T, N) & zObj, const pugi::xml_node& zRoot){\
		from_xml< T, N >(&zObj.x, zRoot);}

#define XML_OXYVEC_ALL( T) \
		XML_OXYVEC( T , 2 )\
		XML_OXYVEC( T , 3 )\
		XML_OXYVEC( T , 4 )

	XML_OXYVEC_ALL(int)
	XML_OXYVEC_ALL(unsigned)
	XML_OXYVEC_ALL(float)
	XML_OXYVEC_ALL(double)
	XML_OXYVEC_ALL(short)
	XML_OXYVEC_ALL(unsigned short)
#undef OVT
}