#pragma once

#include <sstream>
#include <cassert>
#include <vector>
#include <set>
#include <list>
#include <map>
#include <memory>
#include <string>

#include <glm/glm.hpp>
#include <pugixml.hpp>

#include <SDL.h>
#undef main

namespace std
{
	inline const string& to_string(const std::string& v)
	{	
		return v;
	}

	inline const string to_string(const SDL_Rect& v)
	{
		std::ostringstream oss;
		oss << "{" << v.x << "," << v.y << "," << v.w << "," << v.h << "}";
		return oss.str();
	}
}

namespace pgn
{
	typedef pugi::xml_node node_type;

	// Generic functions
	template<class T>
	inline void SerializeOut(node_type& writer, const std::string& key, const T& value) { SerializeOut(writer, key, std::string("unserializable: ") + typeid(T).name()); }

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

#define DECL_SERIALIZE_INTERFACE( T )\
	void SerializeOut(node_type& writer, const std::string& key, const T & value);\
	bool SerializeIn(const node_type& reader, T & value);

#define DECL_SERIAL_CONT1( N ) \
	template<class T> void SerializeOut(pugi::xml_node& writer, const std::string& key, const N < T > & val); \
	template<class T> bool SerializeIn(const pugi::xml_node& reader,  N < T > & val);

#define DECL_SERIAL_CONT2( N ) \
	template<class T0, class T1> void SerializeOut(pugi::xml_node& writer, const std::string& key, const N < T0 , T1 > & val); \
	template<class T0, class T1> bool SerializeIn(const pugi::xml_node& reader, N < T0, T1 > & val);

	// PODs
	DECL_SERIALIZE_INTERFACE(int);
	DECL_SERIALIZE_INTERFACE(float);
	DECL_SERIALIZE_INTERFACE(double);
	DECL_SERIALIZE_INTERFACE(unsigned);
	DECL_SERIALIZE_INTERFACE(bool);
	DECL_SERIALIZE_INTERFACE(std::string);

	// containers
	DECL_SERIAL_CONT1(std::vector);
	DECL_SERIAL_CONT2(std::map);
	DECL_SERIAL_CONT1(std::set);
	DECL_SERIAL_CONT1(std::list);
	DECL_SERIAL_CONT1(glm::detail::tvec2);
	DECL_SERIAL_CONT1(glm::detail::tvec3);
	DECL_SERIAL_CONT1(glm::detail::tvec4);

	// pointers
	DECL_SERIAL_CONT1(std::shared_ptr);

#undef DECL_SERIAL_CONT1
#undef DECL_SERIAL_CONT2
}

#include "serialize_glm.hpp"
#include "serialize_std.hpp"