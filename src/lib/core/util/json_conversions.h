#pragma once

#include <cassert>

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <pystring.h>

#include "logstream.h"
#include "conversions.h"

namespace pgn
{	
	//! Text file to json document (ptr)
	std::shared_ptr<rapidjson::Document> file_to_json(const std::string& zFname);

	//! type(json_doc) to string
	template<>
	inline std::string to_string<rapidjson::Value>( const rapidjson::Value& zRoot)
	{
		rapidjson::StringBuffer strbuf;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(strbuf);
		zRoot.Accept(writer);
		return pystring::strip(strbuf.GetString()," \n");
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
		cLogStream::Default().Wrn( boost::str(boost::format("Type %s does not implement to_json()")% typeid(T).name()));
		//zObj.to_json(zRoot);
	}

	//! type from json
	template <class T>
	inline void from_json(T& zObj, const rapidjson::Value& zRoot)
	{
		cLogStream::Default().Wrn( boost::str(boost::format("Type %s does not implement from_json()")% typeid(T).name()));
		//zObj.from_json(zRoot);
	}

	//! PODs
	template <>
	inline void from_json<std::string>(std::string& zObj, const rapidjson::Value& zRoot)
	{
		assert(zRoot.IsString());
		zObj = zRoot.GetString();
	}

	template <>
	inline void from_json<float>(float& zObj, const rapidjson::Value& zRoot)
	{
		assert(zRoot.IsNumber());
		zObj = float(zRoot.GetDouble());
	}

	template <>
	inline void from_json<double>(double& zObj, const rapidjson::Value& zRoot)
	{
		assert(zRoot.IsNumber());
		zObj = zRoot.GetDouble();
	}

	template <>
	inline void from_json<bool>(bool& zObj, const rapidjson::Value& zRoot)
	{
		assert(zRoot.IsBool());
		zObj = zRoot.GetBool();
	}
}