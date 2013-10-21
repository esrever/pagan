#pragma once

#include <cassert>
#include "json_conversions.h"

namespace pgn
{

	template<class T>
	void read_json(T& obj, const rapidjson::Document& doc)
	{
		asssert(false);
	}

	template<class T>
	void read_json_vector( std::vector<T>& vec, const rapidjson::Value& doc)
	{
		vec.clear();
		T obj;
		if(doc.IsArray())
		{
			vec.reserve( doc.Size());
			for (auto itr = doc.Begin(); itr != doc.End(); ++itr) 
			{
				// get an archetype object
				const auto& robj = *itr;
				from_json(obj, robj);
				vec.push_back(obj);
			}
		}
	}
}