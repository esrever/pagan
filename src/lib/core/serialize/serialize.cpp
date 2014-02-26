#include "serialize.h"

namespace pgn
{
	// PODs
	// int
	template<> void SerializeOut(pugi::xml_node& writer, const std::string& key, const int & val)
	{
		auto& child = writer.append_child(key.c_str());
		child.append_attribute("value").set_value(std::to_string(val).c_str());
	}

	template<> bool SerializeIn(const pugi::xml_node& reader, const std::string& key, int & val, const int & defVal)
	{
		const auto& child = reader.child(key.c_str());
		if (child.empty())
		{
			val = defVal;
			return false;
		}
		else
		{
			val = child.attribute("value").as_int();
			return true;
		}
	}

	// unsigned
	template<> void SerializeOut(pugi::xml_node& writer, const std::string& key, const unsigned & val)
	{
		auto& child = writer.append_child(key.c_str());
		child.append_attribute("value").set_value(std::to_string(val).c_str());
	}

	template<> bool SerializeIn(const pugi::xml_node& reader, const std::string& key, unsigned & val, const unsigned & defVal)
	{
		const auto& child = reader.child(key.c_str());
		if (child.empty())
		{
			val = defVal;
			return false;
		}
		else
		{
			val = child.attribute("value").as_uint();
			return true;
		}
	}

	// float
	template<> void SerializeOut(pugi::xml_node& writer, const std::string& key, const float & val)
	{
		auto& child = writer.append_child(key.c_str());
		child.append_attribute("value").set_value(std::to_string(val).c_str());
	}

	template<> bool SerializeIn(const pugi::xml_node& reader, const std::string& key, float & val, const float & defVal)
	{
		const auto& child = reader.child(key.c_str());
		if (child.empty())
		{
			val = defVal;
			return false;
		}
		else
		{
			val = child.attribute("value").as_float();
			return true;
		}
	}

	// double
	template<> void SerializeOut(pugi::xml_node& writer, const std::string& key, const double & val)
	{
		auto& child = writer.append_child(key.c_str());
		child.append_attribute("value").set_value(std::to_string(val).c_str());
	}

	template<> bool SerializeIn(const pugi::xml_node& reader, const std::string& key, double & val, const double & defVal)
	{
		const auto& child = reader.child(key.c_str());
		if (child.empty())
		{
			val = defVal;
			return false;
		}
		else
		{
			val = child.attribute("value").as_double();
			return true;
		}
	}

	// bool
	template<> void SerializeOut(pugi::xml_node& writer, const std::string& key, const bool & val)
	{
		auto& child = writer.append_child(key.c_str());
		child.append_attribute("value").set_value(std::to_string(val).c_str());
	}

	template<> bool SerializeIn(const pugi::xml_node& reader, const std::string& key, bool & val, const bool & defVal)
	{
		const auto& child = reader.child(key.c_str());
		if (child.empty())
		{
			val = defVal;
			return false;
		}
		else
		{
			val = child.attribute("value").as_bool();
			return true;
		}
	}

	// std::string
	template<> void SerializeOut(pugi::xml_node& writer, const std::string& key, const std::string & val)
	{
		auto& child = writer.append_child(key.c_str());
		child.append_attribute("value").set_value(val.c_str());
	}

	template<> bool SerializeIn(const pugi::xml_node& reader, const std::string& key, std::string & val, const std::string & defVal)
	{
		const auto& child = reader.child(key.c_str());
		if (child.empty())
		{
			val = defVal;
			return false;
		}
		else
		{
			val = child.attribute("value").as_string();
			return true;
		}
	}

	template<class T>
	static void write_glm(pugi::xml_node& writer, const std::string& key, int components, T * val)
	{
		auto& child = writer.append_child(key.c_str());
		const char * s = "xyzw";
		for (int i = 0; i < components; ++i)
		{
			child.append_attribute( std::string(s[i]).c_str()).set_value( std::to_string(val[i]));
		}
	}

	template<class T>
	static void read_glm(pugi::xml_node& writer, const std::string& key, int components, T * val, const T& val)
	{
		const auto& child = reader.child(key.c_str());
		if (child.empty())
		{
			val = defVal;
			return false;
		}
		else
		{
			const char * s = "xyzw";
			for (int i = 0; i < components; ++i)
			{
				const char * v = child.attribute(std::string(s[i]).c_str()).value();
				std::stringstream(v) >> val[i];
			}
			return true;
		}
	}

	// glm::vec2
	// std::string
	template<class T> void SerializeOut(pugi::xml_node& writer, const std::string& key, const glm::detail::tvec2<T> & val)
	{
		write_glm(writer, key, 2, &val.x);
	}

	template<> bool SerializeIn(const pugi::xml_node& reader, const std::string& key, std::string & val, const std::string & defVal)
	{
		read_glm(reader, key, 2, &val.x);
	}
}