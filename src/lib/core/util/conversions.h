#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <memory>

namespace pgn
{
	//! text file to string
	std::string file_to_text(const std::string& fname);

	//! repeats a string by a number of times
	std::string repeat_string(const std::string& s, int count) ;

	//! pad a string by the given text
	std::string pad_string(const std::string& zText, const std::string& zPad);

	//! string to type
	template <class T>
	inline bool from_string(const std::string& s, T& t)
	{
		std::istringstream iss(s);
		return !(iss >> t).fail();
	}

	//! type to string
	template <class T>
	inline std::string to_string(const T& s)
	{
		return "<unknown>";
	}

	#define DEFINE_TOSTRING_POD(T) template <>	inline std::string to_string< T >(const T & s) 	{ return std::to_string(s);}
	DEFINE_TOSTRING_POD(int);
	DEFINE_TOSTRING_POD(unsigned int);
	DEFINE_TOSTRING_POD(long);
	DEFINE_TOSTRING_POD(unsigned long);
	DEFINE_TOSTRING_POD(long double);
	DEFINE_TOSTRING_POD(double);
	DEFINE_TOSTRING_POD(float);

	//! type(string) to string
	template <>
	inline std::string to_string<std::string>(const std::string& s)
	{
		return s;
	}

	//! type(pair) to string
	template<class T, class U>
	inline std::string to_string( const std::pair< T , U >& data)
	{
		std::ostringstream str;
		str << "<"<< to_string(data.first) << ", " << to_string(data.second)<<">";
		return str.str();
	}

	//! type(tuple) to string
	template<class A>
	inline std::string to_string( const std::tuple< A >& data)
	{
		std::ostringstream str;
		str << "<"<< to_string(std::get<0>(data))<<">";
		return str.str();
	}

	//! type(tuple) to string
	template<class A, class B>
	inline std::string to_string( const std::tuple< A , B >& data)
	{
		std::ostringstream str;
		str << "<"<< to_string(std::get<0>(data)) << ", " << to_string(std::get<1>(data))<<">";
		return str.str();
	}

	//! type(tuple) to string
	template<class A, class B, class C>
	inline std::string to_string( const std::tuple< A , B, C >& data)
	{
		std::ostringstream str;
		str << "<"<< to_string(std::get<0>(data)) << ", " << to_string(std::get<1>(data))<<", " << to_string(std::get<2>(data))<<">";
		return str.str();
	}

	//! type(weak_ptr) to string
	template<class T>
	inline std::string to_string( std::weak_ptr< T > data)
	{
		if(auto sptr = data.lock())
			return to_string(sptr);
		else
			return "expired";
	}

	//! type(shared_ptr) to string
	template<class T>
	inline std::string to_string( std::shared_ptr< T > data)
	{
		std::ostringstream str;
		str << "shared_ptr("<< to_string(*data.get()) << ")";
		return str.str();
	}
}