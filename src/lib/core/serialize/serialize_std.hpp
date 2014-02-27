#pragma once

namespace pgn
{
	// vector
	template<class T> inline void SerializeOut(pugi::xml_node& writer, const std::string& key, const std::vector< T > & val)
	{
		auto& child = writer.append_child(key.c_str());
		for (const auto& v : val)
		{
			SerializeOut(child, "array_elem", v);
		}
	}

	template<class T> inline bool SerializeIn(const pugi::xml_node& reader, std::vector < T > & val)
	{
		for (auto it = reader.begin(); it != reader.end(); ++it)
		{
			T v;
			if (SerializeIn(*it, v))
				val.push_back(v);
			else
				return false;

		}
		return true;
	}


	// set
	template<class T> inline void SerializeOut(pugi::xml_node& writer, const std::string& key, const std::set< T > & val)
	{
		auto& child = writer.append_child(key.c_str());
		for (const auto& v : val)
		{
			SerializeOut(child, "set_elem", v);
		}
	}

	template<class T> inline bool SerializeIn(const pugi::xml_node& reader, std::set < T > & val)
	{
		for (auto it = reader.begin(); it != reader.end(); ++it)
		{
			T v;
			if (SerializeIn(*it, v))
				val.insert(v);
			else
				return false;

		}
		return true;
	}

	// map
	template<class T, class U> inline void SerializeOut(pugi::xml_node& writer, const std::string& key, const std::map< T, U > & val)
	{
		auto& child = writer.append_child(key.c_str());
		for (const auto& v : val)
		{
			SerializeOut(child, std::to_string(v.first), v.second);
		}
	}
	
	template<class T, class U> inline bool SerializeIn(const pugi::xml_node& reader, std::map < T, U > & val)
	{
		for (auto it = reader.begin(); it != reader.end(); ++it)
		{
			// read key
			T k;
			std::stringstream(it->name()) >> k;

			// read value
			U v;
			if (SerializeIn(*it, v))
				val.insert(std::make_pair(k,v));
			else
				return false;

		}
		return true;
	}
}