#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <memory>

namespace pgn
{
	template <class T>
	inline bool from_string(const std::string& s, T& t)
	{
		std::istringstream iss(s);
		return !(iss >> t).fail();
	}
	template <class T>
	inline std::string to_string(const T& s)
	{
		return std::to_string(s);
	}
	template <>
	inline std::string to_string<std::string>(const std::string& s)
	{
		return s;
	}

	template<class T, class U>
	inline std::string to_string( const std::pair< T , U >& data)
	{
		std::ostringstream str;
		str << "<"<< to_string(data.first) << ", " << to_string(data.second)<<">";
		return str.str();
	}

	template<class T>
	inline std::string to_string( std::weak_ptr< T > data)
	{
		if(auto sptr = data.lock())
			return to_string(sptr);
		else
			return "expired";
	}

	template<class T>
	inline std::string to_string( std::shared_ptr< T > data)
	{
		std::ostringstream str;
		str << "shared_ptr("<< to_string(*data.get()) << ")";
		return str.str();
	}
}