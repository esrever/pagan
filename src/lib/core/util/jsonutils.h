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
	bool read_json_vector( std::vector<T>& vec, const rapidjson::Value& doc)
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
				if(! from_json(obj, robj))
                    return false;
				vec.push_back(obj);
			}
            return true;
		}
        else
            return false;
	}
}