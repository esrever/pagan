
#pragma once

#include <core/util/json_conversions.h>

namespace pgn
{
	//! 
	struct cInstanceRef
	{
		//! entity info, if it exists
		std::map<cEntity, cEntityComponents>::iterator mEcIter;

		//! name to use to try & find the entity
		std::string mEntityName;
	};

	//-----------------------------------------------------------------------
	template<>
	bool from_json< cInstanceRef>( cInstanceRef& zData, const rapidjson::Value& zRoot);

	//-----------------------------------------------------------------------
	template<>
	void to_json< cInstanceRef>( const cInstanceRef& zData, JsonWriter& zRoot);
}
