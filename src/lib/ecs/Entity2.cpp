#include "Entity.h"

namespace pgn
{
	//---------------------------------------------------------------------------
	cEntity::cEntity():mId(boost::uuids::random_generator()()){}
	
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

	//---------------------------------------------------------------------------
	template<>
	std::string to_string<cEntity>(const cEntity& zEntity)
	{
		return boost::lexical_cast<std::string>(zEntity.Id());
	}
}
