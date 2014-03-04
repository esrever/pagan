#pragma once

namespace pgn
{
	// general
	template<class T>
	static void write_glm(pugi::xml_node& writer, const std::string& key, int components, T * val)
	{
		auto& child = writer.append_child(key.c_str());
		const char * s = "xyzw";
		for (int i = 0; i < components; ++i)
		{
			child.append_attribute(std::string(1,s[i]).c_str()).set_value(std::to_string(val[i]).c_str());
		}
	}

	template<class T>
	static bool read_glm(pugi::xml_node& reader, int components, T * val)
	{
		const char * s = "xyzw";
		for (int i = 0; i < components; ++i)
		{
			const char * v = reader.attribute(std::string(1,s[i]).c_str()).value();
			std::stringstream(v) >> val[i];
		}
		return true;
	}

	// vec2
	template<class T> inline void SerializeOut(pugi::xml_node& writer, const std::string& key, const glm::detail::tvec2<T> & val)
	{
		write_glm(writer, key, 2, &val.x);
	}

	template<class T> inline bool SerializeIn(const pugi::xml_node& reader, glm::detail::tvec2<T>& val)
	{
		read_glm(reader, 2, &val.x);
	}

	// vec3
	template<class T> inline void SerializeOut(pugi::xml_node& writer, const std::string& key, const glm::detail::tvec3<T> & val)
	{
		write_glm(writer, key, 3, &val.x);
	}

	template<class T> inline bool SerializeIn(const pugi::xml_node& reader, glm::detail::tvec3<T>& val)
	{
		read_glm(reader, 3, &val.x);
	}

	// vec4
	template<class T> void inline SerializeOut(pugi::xml_node& writer, const std::string& key, const glm::detail::tvec4<T> & val)
	{
		write_glm(writer, key, 4, &val.x);
	}

	template<class T> bool inline SerializeIn(const pugi::xml_node& reader, glm::detail::tvec4<T>& val)
	{
		read_glm(reader, 4, &val.x);
	}
}