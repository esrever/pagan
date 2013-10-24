#pragma once


#include <glm/glm.hpp>

#include "json_conversions.h"

namespace pgn
{
	template<class T, size_t N>
	bool from_json(T& zObj, const rapidjson::Value& zRoot)
	{
		if(zRoot.IsArray())
		{
			if(zRoot.Size() != N) 
				return false;
			size_t i=0;
			for (auto itr = zRoot.Begin(); itr != zRoot.End(); ++itr) 
			{
				if(!from_json(zObj[i],*itr))
					return false;
				++i
			}
			return true;
		}
		return false;
	}
}