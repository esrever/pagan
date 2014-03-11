#pragma once

#include <cassert>
#include <map>
#include <memory>
#include <set>
#include <vector>
#include <list>
#include <string>
#include <sstream>
#include <type_traits>
#include <pugixml.hpp>

#include <glm/glm.hpp>

#define DECL_SERIALIZE_INTERFACE(T) void   SerializeOut(	  pugi::xml_node& node, const T & value);\
									size_t SerializeIn (const pugi::xml_node& node,		  T & value);

typedef pugi::xml_node node_type;

//! String conversions - TODO: might move them out of here later
namespace std
{
	inline const string& to_string(const std::string& v)
	{
		return v;
	}

	template<class T>
	inline const T from_string(const std::string& v)
	{
		T ret;
		std::istringstream iss(v);
		iss >> ret;
		return ret;
	}
}

namespace pgn
{
	namespace detail
	{
		template<class T>
		struct is_simple { typedef std::integral_constant<bool, false> value_type; };

		template<>struct is_simple<int> { typedef std::integral_constant<bool, true> value_type; };
		template<>struct is_simple<unsigned> { typedef std::integral_constant<bool, true> value_type; };
		template<>struct is_simple<float> { typedef std::integral_constant<bool, true> value_type; };
		template<>struct is_simple<double> { typedef std::integral_constant<bool, true> value_type; };
		template<>struct is_simple<std::string> { typedef std::integral_constant<bool, true> value_type; };
		template<>struct is_simple<bool> { typedef std::integral_constant<bool, true> value_type; };

		//! Attribute readers/writers must support std::to_string
		template<class T>
		inline void WriteAttribute(pugi::xml_node& node, const char * key, const T& value)
		{
			node.append_attribute(key).set_value(value);
		}

		template<>
		inline void WriteAttribute<std::string>(pugi::xml_node& node, const char * key, const std::string& value)
		{
			node.append_attribute(key).set_value(value.c_str());
		}

		template<class T>
		inline bool ReadAttribute(const pugi::xml_node& node, const char * key, T& value)
		{
			auto attrib = node.attribute(key);
			if (!attrib.empty())
				value = std::from_string<T>( attrib.as_string());
			return !attrib.empty();
		}

		template<>
		inline bool ReadAttribute<bool >(const pugi::xml_node& node, const char * key, bool& value)
		{
			auto attrib = node.attribute(key);
			if (!attrib.empty())
				value = attrib.as_bool();
			return !attrib.empty();
		}
	}

	//-------------------------------------------------------------------------------------
	// PODs and default

	template<class T>
	inline void SerializeOut(pugi::xml_node& node, const T& value) { SerializeOut(node, value, detail::is_simple<T>::value_type() ); }

	template<class T>
	inline void SerializeOut(pugi::xml_node& node, const T& value, std::false_type) { SerializeOut(node, std::string("unserializable: ") + typeid(T).name()); }

	template<class T>
	inline void SerializeOut(pugi::xml_node& node, const T& value, std::true_type) { detail::WriteAttribute(node, "value", value); }

	template<class T>
	inline void SerializeOut(pugi::xml_node& node, const char * key, const T& value) { SerializeOut(node, key, value, detail::is_simple<T>::value_type()); }

	template<class T>
	inline void SerializeOut(pugi::xml_node& node, const char * key, const T& value, std::false_type) { SerializeOut(node.append_child(key), value);}	// <------- This automatically creates a child and writes the data, ala struct

	template<class T>
	inline void SerializeOut(pugi::xml_node& node, const char * key, const T& value, std::true_type) { detail::WriteAttribute(node, key, value); }

	// -- Input

	template<class T>
	inline size_t SerializeIn(const pugi::xml_node& node, T& value, const T& def = T()) { size_t ret = SerializeIn(node, value, detail::is_simple<T>::value_type()); if (!ret) value = def; return ret; }

	template<class T>
	inline size_t SerializeIn(const pugi::xml_node& node, T& value, std::false_type) { assert(false); return 0; }

	template<class T>
	inline size_t SerializeIn(const pugi::xml_node& node, T& value, std::true_type) { return detail::ReadAttribute(node, "value", value); }

	template<class T>
	inline size_t SerializeIn(const pugi::xml_node& node, const char * key, T& value, const T& def = T()) { size_t ret = SerializeIn(node, key, value, detail::is_simple<T>::value_type()); if (!ret) value = def; return ret; }

	template<class T>
	inline size_t SerializeIn(const pugi::xml_node& node, const char * key, T& value, std::false_type) { return SerializeIn(node.child(key),value); }

	template<class T>
	inline size_t SerializeIn(const pugi::xml_node& node, const char * key, T& value, std::true_type) { return detail::ReadAttribute(node, key, value); }

	//-------------------------------------------------------------------------------------
	// GLM

	template<class T>
	inline void SerializeOut(pugi::xml_node& node, const glm::detail::tvec2<T>& value)
	{
		detail::WriteAttribute(node,"x", value.x);
		detail::WriteAttribute(node,"y", value.y);
	}

	template<class T>
	inline void SerializeOut(pugi::xml_node& node, const glm::detail::tvec3<T>& value)
	{
		detail::WriteAttribute(node,"x", value.x);
		detail::WriteAttribute(node,"y", value.y);
		detail::WriteAttribute(node,"z", value.z);
	}

	template<class T>
	inline void SerializeOut(pugi::xml_node& node, const glm::detail::tvec4<T>& value)
	{
		detail::WriteAttribute(node,"x", value.x);
		detail::WriteAttribute(node,"y", value.y);
		detail::WriteAttribute(node,"z", value.z);
		detail::WriteAttribute(node,"w", value.w);
	}

	// -- Input

	template<class T>
	inline size_t SerializeIn(const pugi::xml_node& node, glm::detail::tvec2<T>& value)
	{
		size_t ret = 0;
		ret += detail::ReadAttribute(node, "x", value.x);
		ret += detail::ReadAttribute(node, "y", value.y);
		return ret;
	}

	template<class T>
	inline size_t SerializeIn(const pugi::xml_node& node, glm::detail::tvec3<T>& value)
	{
		size_t ret = 0;
		ret += detail::ReadAttribute(node, "x", value.x);
		ret += detail::ReadAttribute(node, "y", value.y);
		ret += detail::ReadAttribute(node, "z", value.z);
		return ret;
	}

	template<class T>
	inline size_t SerializeIn(const pugi::xml_node& node, glm::detail::tvec4<T>& value)
	{
		size_t ret = 0;
		ret += detail::ReadAttribute(node, "x", value.x);
		ret += detail::ReadAttribute(node, "y", value.y);
		ret += detail::ReadAttribute(node, "z", value.z);
		ret += detail::ReadAttribute(node, "w", value.w);
		return ret;
	}


	//-------------------------------------------------------------------------------------
	// STD containers & pair
	template<class T>
	inline void SerializeOut(pugi::xml_node& node, const std::set<T>& value)
	{
		for (const auto& v : value)
			SerializeOut(node.append_child("set_elem"), v);
	}

	template<class T>
	inline void SerializeOut(pugi::xml_node& node, const std::vector<T>& value)
	{
		for (const auto& v : value)
			SerializeOut(node.append_child("vector_elem"), v);
	}

	template<class T>
	inline void SerializeOut(pugi::xml_node& node, const std::list<T>& value)
	{
		for (const auto& v : value)
			SerializeOut(node.append_child("list_elem"), v);
	}

	template<class T, class U>
	inline void SerializeOut(pugi::xml_node& node, const std::map<T, U>& value)
	{
		for (const auto& kv : value)
		{
			SerializeOut(node.append_child("key"), kv.first);
			SerializeOut(node.append_child("value"), kv.second);
		}
	}

	template<class T, class U>
	inline void SerializeOut(pugi::xml_node& node, const std::pair<T, U>& value)
	{
		SerializeOut(node.append_child("first"), value.first);
		SerializeOut(node.append_child("second"), value.second);
	}

	// --- Input

	template<class T>
	inline size_t SerializeIn(const pugi::xml_node& node, std::set<T>& value)
	{
		size_t ret = 0;
		T v;
		for (const auto& n : node.children())
		{
			ret += SerializeIn(n, v);
			value.insert(v);
		}
		return ret;
	}

	template<class T>
	inline size_t SerializeIn(const pugi::xml_node& node, std::vector<T>& value)
	{
		size_t ret = 0;
		T v;
		for (const auto& n : node.children())
		{
			ret += SerializeIn(n, v);
			value.push_back(v);
		}
		return ret;
	}

	template<class T>
	inline size_t SerializeIn(const pugi::xml_node& node, std::list<T>& value)
	{
		size_t ret = 0;
		T v;
		for (const auto& n : node.children())
		{
			ret += SerializeIn(n, v);
			value.push_back(v);
		}
		return ret;
	}

	template<class T, class U>
	inline size_t SerializeIn(const pugi::xml_node& node, std::map<T, U>& value)
	{
		size_t ret = 0;
		T key;
		U val;
		size_t is_val = 0;
		for (const auto& n : node.children())
		{
			if (is_val & 1)
			{
				ret += SerializeIn(n, key);
			}
			else
			{
				ret += SerializeIn(n, val);
				value.emplace(key, val);
				++is_val;
			}
		}
		return ret;
	}

	template<class T, class U>
	inline size_t SerializeIn(const pugi::xml_node& node, std::pair<T, U>& value)
	{
		size_t ret = 0;
		auto n = node.begin();
		ret += SerializeIn(*n, value.first);
		++n;
		ret += SerializeIn(n, value.second);
		return ret;
	}

	//-------------------------------------------------------------------------------------
	// Pointers

	template<class T>
	inline void SerializeOut(pugi::xml_node& node, const std::shared_ptr<T>& value)
	{
		if ( value)
			SerializeOut(node, *value);
		else
			SerializeOut(node, std::string("nullptr"));
	}

	template<class T>
	inline size_t SerializeIn(const pugi::xml_node& node, std::shared_ptr<T>& value)
	{
		size_t ret = 0;
		std::string s;
		detail::ReadAttribute(node, "value", s);
		if (s == "nullptr")
			value.reset();
		else
		{
			T v;
			ret += SerializeIn(node, v);
			value = std::make_shared<T>(v);
		}
		return ret;
	}
}