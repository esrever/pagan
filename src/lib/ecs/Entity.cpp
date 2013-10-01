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
	void from_json<cEntity>(cEntity& zEntity, const rapidjson::Value& zRoot)
	{
		assert(false);
	}

	//---------------------------------------------------------------------------
	template<>
	std::string to_string<cEntity>(const cEntity& zEntity)
	{
		return boost::lexical_cast<std::string>(zEntity.Id());
	}
}

//bool std::less<pgn::cEntityWptr>::operator ()(const pgn::cEntityWptr &lhs ,const pgn::cEntityWptr &rhs) const
/*
bool operator < (const pgn::cEntity &lhs ,const pgn::cEntity &rhs)
{
	return lhs.Id() < rhs.Id(); 
}

bool operator < (const pgn::cEntityWptr &lhs ,const pgn::cEntityWptr &rhs)
{
	return *lhs.lock() < *rhs.lock(); 
}
*/