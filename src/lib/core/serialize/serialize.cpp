#include "serialize.h"

namespace pgn
{
	// PODs
	// General
	template<class T>
	static bool read_pod(const pugi::xml_node& child, T& v)
	{
		const auto& attr = child.attribute("value");
		if (attr.empty())
			return false;
		else
		{
			std::stringstream(attr.as_string()) >> v;
			return true;
		}
	}
	
	// int
	void SerializeOut(pugi::xml_node& writer, const std::string& key, const int & val)
	{
		auto& child = writer.append_child(key.c_str());
		child.append_attribute("value").set_value(std::to_string(val).c_str());
	}

	bool SerializeIn(const pugi::xml_node& child, int & val)
	{
		return read_pod(child, val);
	}

	// unsigned
	void SerializeOut(pugi::xml_node& writer, const std::string& key, const unsigned & val)
	{
		auto& child = writer.append_child(key.c_str());
		child.append_attribute("value").set_value(std::to_string(val).c_str());
	}

	bool SerializeIn(const pugi::xml_node& child, unsigned & val)
	{
		return read_pod(child, val);
	}

	// float
	void SerializeOut(pugi::xml_node& writer, const std::string& key, const float & val)
	{
		auto& child = writer.append_child(key.c_str());
		child.append_attribute("value").set_value(std::to_string(val).c_str());
	}

	bool SerializeIn(const pugi::xml_node& child, float & val)
	{
		return read_pod(child, val);
	}

	// double
	void SerializeOut(pugi::xml_node& writer, const std::string& key, const double & val)
	{
		auto& child = writer.append_child(key.c_str());
		child.append_attribute("value").set_value(std::to_string(val).c_str());
	}

	bool SerializeIn(const pugi::xml_node& child, double & val)
	{
		return read_pod(child, val);
	}

	// bool
	void SerializeOut(pugi::xml_node& writer, const std::string& key, const bool & val)
	{
		auto& child = writer.append_child(key.c_str());
		child.append_attribute("value").set_value(std::to_string(val).c_str());
	}

	bool SerializeIn(const pugi::xml_node& child, bool & val)
	{
		return read_pod(child, val);
	}

	// std::string
	void SerializeOut(pugi::xml_node& writer, const std::string& key, const std::string & val)
	{
		auto& child = writer.append_child(key.c_str());
		child.append_attribute("value").set_value(val.c_str());
	}

	bool SerializeIn(const pugi::xml_node& reader, std::string & val)
	{
		return read_pod(reader, val);
	}
}