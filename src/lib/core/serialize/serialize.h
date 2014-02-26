#pragma once

#include <sstream>
#include <cassert>
#include <vector>
#include <set>
#include <map>
#include <string>

#include <glm/glm.hpp>

#include <pugixml.hpp>

namespace pgn
{
	typedef pugi::xml_node node_type;

	// Generic functions
	template<class T>
	inline void SerializeOut(node_type& writer, const std::string& key, const T& value) { assert(false); }

	template<class T>
	inline bool SerializeIn(const node_type& reader, const std::string& key, T& value) { assert(false); return false; }

	template<class T>
	inline bool SerializeIn(const node_type& reader, const std::string& key, T& value, const T& defVal = T())
	{
		const auto& child = reader.child(key.c_str());
		if (child.empty())
		{
			val = defVal;
			return false;
		}
		else
		{
			return SerializeIn(reader, key, value);
		}
	}

#define DECL_SERIAL_SPECIAL( T ) \
	template<> void SerializeOut(pugi::xml_node& writer, const std::string& key, const T & val);\
	template<> bool SerializeIn(const pugi::xml_node& writer, const std::string& key, T & val, const T & defVal);

#define DECL_SERIAL_CONT1( N ) \
	template<class T> void SerializeOut(pugi::xml_node& writer, const std::string& key, const N < T > & val); \
	template<class T> bool SerializeIn(const pugi::xml_node& writer, const std::string& key, N < T > & val, const N < T > & defVal);

#define DECL_SERIAL_CONT2( N ) \
	template<class T0, class T1> void SerializeOut(pugi::xml_node& writer, const std::string& key, const N < T0 , T1 > & val); \
	template<class T0, class T1> bool SerializeIn(const pugi::xml_node& writer, const std::string& key, N < T0 , T1 > & val, const N < T0 , T1 > & defVal);

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
