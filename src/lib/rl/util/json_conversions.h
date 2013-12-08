#pragma once

#include <oxygine-framework.h>
#include <pystring.h>
#include <core/util/json_conversions.h>

namespace pgn
{
	template< class T >
	inline bool from_json(oxygine::intrusive_ptr<T>& zData, const rapidjson::Value& zRoot)
	{
		return zData ? from_json(*zData, zRoot) : false;
	}

	template< class T >
	inline void to_json(oxygine::intrusive_ptr<T>& zData, const rapidjson::Value& zRoot)
	{
		zData ? to_json(*zData, zRoot) : 0;
	}
	
		
	template<>
	inline bool from_json< oxygine::TextStyle::HorizontalAlign >(oxygine::TextStyle::HorizontalAlign & zData, const rapidjson::Value& zRoot)
	{
		zData = oxygine::TextStyle::HALIGN_DEFAULT;
		std::string s;
		from_json(s, zRoot);
		if (s == "right")
			zData = oxygine::TextStyle::HALIGN_RIGHT;
		else if (s == "center")
			zData = oxygine::TextStyle::HALIGN_CENTER;
		else if (s == "left")
			zData = oxygine::TextStyle::HALIGN_LEFT;
		return true;
	}

	template<>
	inline void to_json< oxygine::TextStyle::HorizontalAlign >(const oxygine::TextStyle::HorizontalAlign & zData, JsonWriter& zRoot)
	{
		switch (zData)
		{
			case oxygine::TextStyle::HALIGN_DEFAULT: to_json("default", zRoot); break;
			case oxygine::TextStyle::HALIGN_RIGHT: to_json("right", zRoot); break;
			case oxygine::TextStyle::HALIGN_LEFT: to_json("left", zRoot); break;
			case oxygine::TextStyle::HALIGN_CENTER: to_json("center", zRoot); break;
		};
	}

	template<>
	inline bool from_json< oxygine::TextStyle::VerticalAlign >(oxygine::TextStyle::VerticalAlign & zData, const rapidjson::Value& zRoot)
	{
		zData = oxygine::TextStyle::VALIGN_DEFAULT;
		std::string s;
		from_json(s, zRoot);
		if (s == "baseline")
			zData = oxygine::TextStyle::VALIGN_BASELINE;
		else if (s == "top")
			zData = oxygine::TextStyle::VALIGN_TOP;
		else if (s == "middle")
			zData = oxygine::TextStyle::VALIGN_MIDDLE;
		else if (s == "bottom")
			zData = oxygine::TextStyle::VALIGN_BOTTOM;
		return true;
	}

	template<>
	inline void to_json< oxygine::TextStyle::VerticalAlign >(const oxygine::TextStyle::VerticalAlign & zData, JsonWriter& zRoot)
	{
		switch (zData)
		{
		case oxygine::TextStyle::VALIGN_DEFAULT: to_json("default", zRoot); break;
		case oxygine::TextStyle::VALIGN_BASELINE: to_json("baseline", zRoot); break;
		case oxygine::TextStyle::VALIGN_TOP: to_json("top", zRoot); break;
		case oxygine::TextStyle::VALIGN_MIDDLE: to_json("middle", zRoot); break;
		case oxygine::TextStyle::VALIGN_BOTTOM: to_json("bottom", zRoot); break;
		};
	}

#define OVT( T , N ) oxygine::VectorT##N##< T >
#define JSON_OXYVEC( T, N ) \
	template <>\
	inline void to_json< OVT ( T , N ) >(const  OVT ( T , N ) & zObj, JsonWriter& zRoot){\
		to_json< T , N >(&zObj.x, zRoot);}\
	template <>\
	inline bool from_json< OVT ( T , N ) >( OVT ( T , N ) & zObj, const rapidjson::Value& zRoot){\
		return from_json< T , N >(&zObj.x, zRoot);}

#define JSON_OXYVEC_ALL( T) \
	JSON_OXYVEC( T , 2)\
	JSON_OXYVEC( T , 3)\
	JSON_OXYVEC( T , 4)

		JSON_OXYVEC_ALL(int)
		JSON_OXYVEC_ALL(unsigned)
		JSON_OXYVEC_ALL(short)
		JSON_OXYVEC_ALL(unsigned short)
		JSON_OXYVEC_ALL(float)
		JSON_OXYVEC_ALL(double)
		JSON_OXYVEC_ALL(char)
		JSON_OXYVEC_ALL(unsigned char)
#undef OVT

	DECL_JSON_PGN(::oxygine::TextStyle)
	DECL_JSON_PGN(::oxygine::ColorRectSprite)
	DECL_JSON_PGN(::oxygine::TextActor)
	DECL_JSON_PGN(::oxygine::Color)
	
}