#pragma once

#include <string>
#include <iostream>
#include <sstream>

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
}