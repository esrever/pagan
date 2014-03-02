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

	// list
	template<class T> inline void SerializeOut(pugi::xml_node& writer, const std::string& key, const std::list< T > & val)
	{
		auto& child = writer.append_child(key.c_str());
		for (const auto& v : val)
		{
			SerializeOut(child, "list_elem", v);
		}
	}

	template<class T> inline bool SerializeIn(const pugi::xml_node& reader, std::list < T > & val)
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

	// map
	template<class T, class U> inline void SerializeOut(pugi::xml_node& writer, const std::string& key, const std::map< T, U > & val)
	{
		auto& child = writer.append_child(key.c_str());
		for (const auto& v : val)
		{
			//SerializeOut(child, std::to_string(v.first), v.second);
			SerializeOut(child, "map_key", v.first);
			SerializeOut(child, "map_value", v.second);
		}
	}
	
	template<class T, class U> inline bool SerializeIn(const pugi::xml_node& reader, std::map < T, U > & mval)
	{
		size_t i = 0;
		T key, U val;
		for (auto it = reader.begin(); it != reader.end(); ++it)
		{
			bool ok = true;
			if (i & 1)
			{
				ok = SerializeIn(reader, val);
				if (ok)
					mval[key] = val;
			}
			else
				ok = SerializeIn(reader, key);
			if (!ok)
				return false;
			++i;

		}
		return true;
	}


	// pointers
	template<class T> inline void SerializeOut(pugi::xml_node& writer, const std::string& key, const std::shared_ptr< T > & val)
	{
		if (val)
			SerializeOut(writer, key, *val);
		else
			SerializeOut(writer, key, std::string("null"));
	}

	template<class T> inline bool SerializeIn(const pugi::xml_node& reader, std::shared_ptr < T > & pval)
	{
		T val;
		if (SerializeIn(reader, val))
		{
			pval = std::make_shared(val);
			return true;
		}
		else
			return false;
	}
}