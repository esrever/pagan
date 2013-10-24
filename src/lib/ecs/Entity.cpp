#include "Entity.h"

namespace pgn
{	
	//---------------------------------------------------------------------------
	template<>
	void to_json<cEntity>(const cEntity& zEntity, rapidjson::Value& zRoot)
	{
		assert(false);
	}
	
	//---------------------------------------------------------------------------
	template<>
	bool from_json<cEntity>(cEntity& zEntity, const rapidjson::Value& zRoot)
	{
		assert(false);
        return false;
	}
}
