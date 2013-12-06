#pragma once

#include <cassert>
#include <vector>
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

	//! type to xml
	template <class T>
	inline void to_xml(const T& zObj, pugi::xml_attribute& node)
	{
		cLogStream::Default().Wrn(boost::str(boost::format("Type %s does not implement to_xml()")% typeid(T).name()));
	}

	//! type from xml
	template <class T>
	inline void from_xml(T& zObj, const pugi::xml_attribute& zRoot)
	{
		cLogStream::Default().Wrn(boost::str(boost::format("Type %s does not implement from_xml()")% typeid(T).name()));
	}

	//! PODs
	// char
	template <>
	inline void from_xml<char>(char& zObj, const pugi::xml_attribute& zRoot)
	{
		zObj = zRoot.as_string("")[0];
	}

	template <>
	inline void to_xml<char>(const char& zObj, pugi::xml_attribute& zRoot)
	{
		std::string s(1, zObj);
		zRoot.set_value(s.c_str());
	}

	// int
	template <>
	inline void from_xml<int>(int& zObj, const pugi::xml_attribute& zRoot)
	{
		zObj = zRoot.as_int(-1);
	}

	template <>
	inline void to_xml<int>(const int& zObj, pugi::xml_attribute& zRoot)
	{
		zRoot.set_value(zObj);
	}

	// uint
	template <>
	inline void from_xml<unsigned int>(unsigned int& zObj, const pugi::xml_attribute& zRoot)
	{
		zObj = zRoot.as_uint(0xFFFFFFFF);
	}

	template <>
	inline void to_xml<unsigned int>(const unsigned & zObj, pugi::xml_attribute& zRoot)
	{
		zRoot.set_value(zObj);
	}

	// short
	template <>
	inline void from_xml<short>(short& zObj, const pugi::xml_attribute& zRoot)
	{
		int v;
		from_xml(v, zRoot);
		zObj = static_cast<short>(v);
	}

	template <>
	inline void to_xml<short>(const short& zObj, pugi::xml_attribute& zRoot)
	{
		zRoot.set_value(int(zObj));
	}

	// ushort
	template <>
	inline void from_xml<unsigned short>(unsigned short& zObj, const pugi::xml_attribute& zRoot)
	{
		unsigned v;
		from_xml(v, zRoot);
		zObj = static_cast<unsigned short>(v);
	}

	template <>
	inline void to_xml<unsigned short>(const unsigned short & zObj, pugi::xml_attribute& zRoot)
	{
		zRoot.set_value(unsigned(zObj));
	}

	// float
	template <>
	inline void from_xml<float>(float& zObj, const pugi::xml_attribute& zRoot)
	{
		zObj = zRoot.as_float(-1.0f);
	}

	template <>
	inline void to_xml<float>(const float & zObj, pugi::xml_attribute& zRoot)
	{
		zRoot.set_value(zObj);
	}

	// double
	template <>
	inline void from_xml<double>(double& zObj, const pugi::xml_attribute& zRoot)
	{
		zObj = zRoot.as_double(-1);
	}

	template <>
	inline void to_xml<double>(const double & zObj, pugi::xml_attribute& zRoot)
	{
		zRoot.set_value(zObj);
	}

	// std::string
	template <>
	inline void from_xml<std::string>(std::string& zObj, const pugi::xml_attribute& zRoot)
	{
		zObj = zRoot.as_string("");
	}

	template <>
	inline void to_xml<std::string>(const std::string & zObj, pugi::xml_attribute& zRoot)
	{
		zRoot.set_value(zObj.c_str());
	}

	// bool
	template <>
	inline void from_xml<bool>(bool& zObj, const pugi::xml_attribute& zRoot)
	{
		zObj = zRoot.as_bool();
	}

	template <>
	inline void to_xml<bool>(const bool & zObj, pugi::xml_attribute& zRoot)
	{
		zRoot.set_value(zObj);
	}
	//! Containers

	// vector
	template <class T>
	inline void from_xml(std::vector<T>& zObj, const pugi::xml_node& zRoot)
	{
		zObj.clear();
		T val;
		for (auto c : zRoot)
		{
			from_xml(val, c.attribute("value"));
			zObj.push_back(val);
		}
	}

	template <class T>
	inline void to_xml(const std::vector<T> & zObj, pugi::xml_node& zRoot)
	{
		for (const auto& x : zObj)
		{
			auto node& = zRoot.append_child("elem");
			auto& attrib = node.append_attribute("value");
			to_xml(x, attrib);
		}
	}

	// set
	template <class T>
	inline void from_xml(std::set<T>& zObj, const pugi::xml_node& zRoot)
	{
		zObj.clear();
		T val;
		for (auto c : zRoot)
		{
			from_xml(val, c.attribute("value"));
			zObj.push_back(val);
		}
	}

	template <class T>
	inline void to_xml(const std::set<T> & zObj, pugi::xml_node& zRoot)
	{
		for (const auto& x : zObj)
		{
			auto node& = zRoot.append_child("elem");
			auto& attrib = node.append_attribute("value");
			to_xml(x, attrib);
		}
	}

	// map
	template <class T,class U>
	inline void from_xml(std::map<T,U>& zObj, const pugi::xml_node& zRoot)
	{
		zObj.clear();
		T val; U key;
		for (auto c : zRoot)
		{
			from_xml(key, c.attribute("key"));
			from_xml(val, c.attribute("value"));
			zObj.push_back(val);
		}
	}

	template <class T,class U>
	inline void to_xml(const std::map<T, U> & zObj, pugi::xml_node& zRoot)
	{
		for (const auto& x : zObj)
		{
			auto node& = zRoot.append_child("elem");
			auto& kattrib = node.append_attribute("key");
			auto& vattrib = node.append_attribute("value");
			to_xml(x.first, kattrib);
			to_xml(x.second, vattrib);
		}
	}

	// multimap
	template <class T, class U>
	inline void from_xml(std::multimap<T, U>& zObj, const pugi::xml_node& zRoot)
	{
		zObj.clear();
		T val; U key;
		for (auto c : zRoot)
		{
			from_xml(key, c.attribute("key"));
			from_xml(val, c.attribute("value"));
			zObj.push_back(val);
		}
	}

	template <class T, class U>
	inline void to_xml(const std::multimap<T, U> & zObj, pugi::xml_node& zRoot)
	{
		for (const auto& x : zObj)
		{
			auto node& = zRoot.append_child("elem");
			auto& kattrib = node.append_attribute("key");
			auto& vattrib = node.append_attribute("value");
			to_xml(x.first, kattrib);
			to_xml(x.second, vattrib);
		}
	}

	// shared_ptr
	template <class T>
	inline void from_xml(std::shared_ptr<T>& zObj, const pugi::xml_attribute& zRoot)
	{
		T val;
		from_xml(val, zRoot);
		zObj = std::make_shared<T>(val);
	}

	template <class T>
	inline void to_xml(const std::shared_ptr<T>& zObj, pugi::xml_attribute& zRoot)
	{
		zObj ? to_xml(*zObj, zRoot) : zRoot.set_value("nullptr");
	}

	// weak_ptr
	template <class T>
	inline void to_xml(const std::weak_ptr<T>& zObj, pugi::xml_attribute& zRoot)
	{
		to_xml(zObj.lock(), zRoot);
	}

	/*
		glm 
		oxygine-related (sep. file)
	*/
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