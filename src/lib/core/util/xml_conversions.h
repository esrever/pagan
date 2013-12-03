#pragma once

#include <cassert>
#include <set>
#include <map>
#include <memory>

#include <pugixml.hpp>

namespace pgn
{
	//! Text file to json document (ptr)
	std::shared_ptr<pugi::xml_document> file_to_xml(const std::string& zFname);

	template<class T>
	inline void xml_add_child(const std::string& zName, const T& zValue, pugi::xml_node& node)
	{
		auto cnode = node.append_child(zName.c_str());
		to_xml(zValue, cnode);
	}
}

/*

namespace pgn
{	
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

	template <>
	inline bool from_json<int>(int& zObj, const rapidjson::Value& zRoot)
	{
		if (zRoot.IsNumber())
			zObj = unsigned(zRoot.GetInt());
		return zRoot.IsNumber();
	}
	template <>
	inline void to_json<int>(const int& zObj, JsonWriter& zRoot)
	{
		zRoot.Int(zObj);
	}

	//-------------------------------------------------------------------------
	template <class T, size_t N>
	inline void to_json(const T * zObj, JsonWriter& zRoot)
	{
		zRoot.StartArray();
		for (size_t i = 0; i < N; ++i)
			to_json(zObj[i], zRoot);
		zRoot.EndArray();
	}

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

	#define JSON_GLM( T, N, P) \
	template <>\
	inline void to_json<glm::##T##N >(const glm::##T##N& zObj, JsonWriter& zRoot){\
		to_json< P , N >(&zObj.x, zRoot);}\
	template <>\
	inline bool from_json<glm::##T##N >(glm::##T##N& zObj, const rapidjson::Value& zRoot){\
		return from_json<glm::##T##N ,  N >(zObj, zRoot);\
	}

	#define JSON_GLM_ALL( T, P) \
		JSON_GLM(T,2,P)\
		JSON_GLM(T, 3, P)\
		JSON_GLM(T, 4, P)

	JSON_GLM_ALL(ivec, int)
	JSON_GLM_ALL(uvec, unsigned)
	JSON_GLM_ALL(vec, float)
}
*/