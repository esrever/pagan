#pragma once

#include <cassert>

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <pystring.h>

#include "conversions.h"


namespace pgn
{
	//! type(json_doc) to string
	template<>
	inline std::string to_string<rapidjson::Value>( const rapidjson::Value& zRoot)
	{
		assert(false);
		return std::string();
	}

	//! string to type(json)
	template <>
	inline bool from_string<rapidjson::Value>(const std::string& s, rapidjson::Value& t)
	{
		assert(false);
		return false;
	}

	//! type to json
	template <class T>
	inline void to_json(const T& zObj, rapidjson::Value& zRoot)
	{
		assert(false);
	}

	//! type from json
	template <class T>
	inline void from_json(T& zObj, const rapidjson::Value& zRoot)
	{
		assert(false);
	}
}