#pragma once

#include <cassert>
#include <set>
#include <map>

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <pystring.h>
#include <glm/glm.hpp>

#include "logstream.h"
#include "conversions.h"

typedef rapidjson::PrettyWriter<rapidjson::StringBuffer> JsonWriter;

namespace pgn
{	
	template<class T>
	inline void JsonWriter_AddMember(const std::string& zName, const T& zValue, JsonWriter& zWriter)
	{
		zWriter.String(zName.c_str());
		to_json(zValue, zWriter);
	}

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
	inline void to_json(const T& zObj, JsonWriter& zRoot)
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
	inline void to_json<std::string>(const std::string& zObj, JsonWriter& zRoot)
	{
		zRoot.String( zObj.c_str());
	}

	template <>
	inline bool from_json<char>(char& zObj, const rapidjson::Value& zRoot)
	{
		if(zRoot.IsString())
			zObj = zRoot.GetString()[0];
        return zRoot.IsString();
	}

	template <>
	inline void to_json<char>(const char& zObj, JsonWriter& zRoot)
	{
		char tmp[2] = {zObj,'\n'};
		zRoot.String( tmp);
	}

	template <>
	inline bool from_json<float>(float& zObj, const rapidjson::Value& zRoot)
	{
		if(zRoot.IsNumber())
			zObj = float(zRoot.GetDouble());
        return zRoot.IsNumber();
	}
	template <>
	inline void to_json<float>(const float& zObj, JsonWriter& zRoot)
	{
		zRoot.Double( zObj);
	}

	template <>
	inline bool from_json<double>(double& zObj, const rapidjson::Value& zRoot)
	{
		if(zRoot.IsNumber())
			zObj = zRoot.GetDouble();
        return zRoot.IsNumber();
	}
	template <>
	inline void to_json<double>(const double& zObj, JsonWriter& zRoot)
	{
		zRoot.Double( zObj);
	}

	template <>
	inline bool from_json<bool>(bool& zObj, const rapidjson::Value& zRoot)
	{
		if(zRoot.IsBool())
			zObj = zRoot.GetBool();
        return zRoot.IsBool();
	}

	template <>
	inline void to_json<bool>(const bool& zObj, JsonWriter& zRoot)
	{
		zRoot.Bool(zObj);
	}

	template <>
	inline bool from_json<unsigned short>(unsigned short& zObj, const rapidjson::Value& zRoot)
	{
		if(zRoot.IsNumber())
			zObj = unsigned short(zRoot.GetUint());
        return zRoot.IsNumber();
	}
	template <>
	inline void to_json<unsigned short>(const unsigned short& zObj, JsonWriter& zRoot)
	{
		zRoot.Uint( zObj);
	}
	
	template <>
	inline bool from_json<unsigned int>(unsigned int& zObj, const rapidjson::Value& zRoot)
	{
		if(zRoot.IsNumber())
			zObj = unsigned(zRoot.GetUint());
        return zRoot.IsNumber();
	}
	template <>
	inline void to_json<unsigned int>(const unsigned int& zObj, JsonWriter& zRoot)
	{
		zRoot.Uint( zObj);
	}

	//-------------------------------------------------------------------------
	template <class T>
	inline void to_json(const std::vector<T>& zObj, JsonWriter& zRoot)
	{
		zRoot.StartArray();
		for (const auto& x : zObj)
			to_json(x, zRoot);
		zRoot.EndArray();
	}

	template <class T>
	inline void to_json(const std::set<T>& zObj, JsonWriter& zRoot)
	{
		zRoot.StartArray();
		for (const auto& x : zObj)
			to_json(x, zRoot);
		zRoot.EndArray();
	}

	template <class T, class U>
	inline void to_json(const std::map<T,U>& zObj, JsonWriter& zRoot)
	{
		zRoot.StartObject();
		for (const auto& x : zObj)
		{
			zRoot.String( to_string(x.first).c_str());
			to_json(x.second,zRoot);
		}
		zRoot.EndObject();
	}

	template <class T, class U>
	inline void to_json(const std::multimap<T, U>& zObj, JsonWriter& zRoot)
	{
		zRoot.StartArray();
		for (const auto& x : zObj)
		{
			zRoot.StartObject();
			JsonWriter_AddMember(to_string(x.first), x.second, zRoot);
			zRoot.EndObject();
		}
		zRoot.EndArray();
	}

	template <class T>
	inline void to_json(const std::shared_ptr<T>& zObj, JsonWriter& zRoot)
	{
		zObj ? to_json(*zObj, zRoot) : zRoot.Null();
	}

	//-------------------------------------------------------------------------------------
	template<class T, size_t N>
	bool from_json(T& zObj, const rapidjson::Value& zRoot)
	{
		if(zRoot.IsArray())
		{
			if(zRoot.Size() != N) 
				return false;
			size_t i=0;
			for (auto itr = zRoot.Begin(); itr != zRoot.End(); ++itr) 
			{
				if(!from_json(zObj[i],*itr))
					return false;
				++i;
			}
			return true;
		}
		return false;
	}

	template <>
	inline bool from_json<glm::uvec2>(glm::uvec2& zObj, const rapidjson::Value& zRoot)
	{
		return from_json<glm::uvec2,2>(zObj,zRoot);
	}

	template <>
	inline bool from_json<glm::ivec2>(glm::ivec2& zObj, const rapidjson::Value& zRoot)
	{
		return from_json<glm::ivec2,2>(zObj,zRoot);
	}
}