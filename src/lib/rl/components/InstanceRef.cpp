
#include "InstanceRef.h"

namespace pgn
{
	//-----------------------------------------------------------------------
	template<>
	bool from_json< cInstanceRef>( cInstanceRef& zData, const rapidjson::Value& zRoot)
	{
		return true;
	}

	//-----------------------------------------------------------------------
	template<>
	void to_json< cInstanceRef>( const cInstanceRef& zData, JsonWriter& zRoot)
	{
		zRoot.StartObject();
		zRoot.EndObject();
	}
}
