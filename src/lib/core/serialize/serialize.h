#pragma once

#include <sstream>
#include <cassert>
#include <vector>
#include <set>
#include <map>
#include <string>

#include <glm/glm.hpp>

#include <pugixml.hpp>

namespace std
{
	inline const string& to_string(const std::string& v)
	{	
		return v;
	}
}

namespace pgn
{
	typedef pugi::xml_node node_type;

	// Generic functions
	template<class T>
	inline void SerializeOut(node_type& writer, const std::string& key, const T& value) { assert(false); }

	template<class T>
	inline bool SerializeIn(const node_type& reader, T& value) { assert(false); return false; }

	template<class T>
	inline bool SerializeIn(const node_type& reader, const std::string& key, T& value, const T& defVal = T())
	{
		const auto& child = reader.child(key.c_str());
		if (child.empty()) 
			return false;
		bool ok = SerializeIn(child, value);
		if (!ok) 
			value = defVal;
		return ok;
	}

#define DECL_SERIAL_SPECIAL( T ) \
	template<> void SerializeOut(pugi::xml_node& writer, const std::string& key, const T & val);\
	template<> bool SerializeIn(const pugi::xml_node& reader, T & val);

#define DECL_SERIAL_CONT1( N ) \
	template<class T> void SerializeOut(pugi::xml_node& writer, const std::string& key, const N < T > & val); \
	template<class T> bool SerializeIn(const pugi::xml_node& reader,  N < T > & val);

#define DECL_SERIAL_CONT2( N ) \
	template<class T0, class T1> void SerializeOut(pugi::xml_node& writer, const std::string& key, const N < T0 , T1 > & val); \
	template<class T0, class T1> bool SerializeIn(const pugi::xml_node& reader, N < T0, T1 > & val);

	// PODs
	DECL_SERIAL_SPECIAL(int);
	DECL_SERIAL_SPECIAL(float);
	DECL_SERIAL_SPECIAL(double);
	DECL_SERIAL_SPECIAL(unsigned);
	DECL_SERIAL_SPECIAL(bool);
	DECL_SERIAL_SPECIAL(std::string);

	// containers
	DECL_SERIAL_CONT1(std::vector);
	DECL_SERIAL_CONT2(std::map);
	DECL_SERIAL_CONT1(std::set);
	DECL_SERIAL_CONT1(glm::detail::tvec2);
	DECL_SERIAL_CONT1(glm::detail::tvec3);
	DECL_SERIAL_CONT1(glm::detail::tvec4);

#undef DECL_SERIAL_SPECIAL
#undef DECL_SERIAL_CONT1
#undef DECL_SERIAL_CONT2
}

#include "serialize_glm.hpp"
#include "serialize_std.hpp"