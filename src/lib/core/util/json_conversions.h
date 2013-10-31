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
	inline bool from_json(T& zObj, const rapidjson::Value& zRoot)
	{
		cLogStream::Default().Wrn( boost::str(boost::format("Type %s does not implement from_json()")% typeid(T).name()));
		return false;
	}

	//! PODs
	template <>
	inline bool from_json<std::string>(std::string& zObj, const rapidjson::Value& zRoot)
	{
		if(zRoot.IsString())
			zObj = zRoot.GetString();
        return zRoot.IsString();
	}

	template <>
	inline void to_json<std::string>(const std::string& zObj, rapidjson::Value& zRoot)
	{
		zRoot.SetString( zObj.c_str());
	}

	template <>
	inline bool from_json<char>(char& zObj, const rapidjson::Value& zRoot)
	{
		if(zRoot.IsString())
			zObj = zRoot.GetString()[0];
        return zRoot.IsString();
	}

	template <>
	inline void to_json<char>(const char& zObj, rapidjson::Value& zRoot)
	{
		char tmp[2] = {zObj,'\n'};
		zRoot.SetString( tmp);
	}

	template <>
	inline bool from_json<float>(float& zObj, const rapidjson::Value& zRoot)
	{
		if(zRoot.IsNumber())
			zObj = float(zRoot.GetDouble());
        return zRoot.IsNumber();
	}
	template <>
	inline void to_json<float>(const float& zObj, rapidjson::Value& zRoot)
	{
		zRoot.SetDouble( zObj);
	}

	template <>
	inline bool from_json<double>(double& zObj, const rapidjson::Value& zRoot)
	{
		if(zRoot.IsNumber())
			zObj = zRoot.GetDouble();
        return zRoot.IsNumber();
	}
	template <>
	inline void to_json<double>(const double& zObj, rapidjson::Value& zRoot)
	{
		zRoot.SetDouble( zObj);
	}

	template <>
	inline bool from_json<bool>(bool& zObj, const rapidjson::Value& zRoot)
	{
		if(zRoot.IsBool())
			zObj = zRoot.GetBool();
        return zRoot.IsBool();
	}

	template <>
	inline void to_json<bool>(const bool& zObj, rapidjson::Value& zRoot)
	{
		zRoot.SetBool(zObj);
	}

	template <>
	inline bool from_json<unsigned short>(unsigned short& zObj, const rapidjson::Value& zRoot)
	{
		if(zRoot.IsNumber())
			zObj = unsigned short(zRoot.GetUint());
        return zRoot.IsNumber();
	}
	template <>
	inline void to_json<unsigned short>(const unsigned short& zObj, rapidjson::Value& zRoot)
	{
		zRoot.SetUint( zObj);
	}
}