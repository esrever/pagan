#pragma once
/*
#include <iostream>
#include <functional>
#include <map>
#include <string>
#include <typeinfo>
#include <typeindex>
#include <cassert>

#define DECL_VAR( T )\
public:\
template<>\
std::map<std::string, T >& Get< T >() { return m##T; }\
template<class T>\
const std::map<std::string, T>& Get() const{ assert(false); static std::map<std::string, T> v; return v; }\
private:\
	std::map<std::string, T > m##T;

namespace pgn
{
	template<typename... Args>
	class cDict2
	{
		
	};
}

*/