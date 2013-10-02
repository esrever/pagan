#pragma once

#include <bitset>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>        
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <core/util/conversions.h>
#include <core/util/json_conversions.h>

#include "ecs_config.h"
#include "Event.h"

namespace pgn 
{
	class cEntityMgr;

    //! 
    class cEntity
    {
	public:
		//! get uuid
		const boost::uuids::uuid& Id() const {return mId;} 
		//! compare uuids
		bool operator < (const pgn::cEntity &rhs) const {return Id() < rhs.Id();}
	private:
		friend class cEntityMgr;
		cEntity();

	private:
		boost::uuids::uuid mId;
    };

	DECL_PTRTYPE(cEntity)

	DECL_EVENT(EntityCreated, cEntityWptr);
	DECL_EVENT(DestroyEntity, cEntityWptr);

	//! entity to json
	template<>
	void to_json<cEntity>(const cEntity& zEntity, rapidjson::Value& zRoot);
	//! entity from json
	template<>
	void from_json<cEntity>(cEntity& zEntity, const rapidjson::Value& zRoot);
	//! entity to string
	template<>
	std::string to_string<cEntity>(const cEntity& zEntity);
}

//! Global wptr comparison operator 
inline bool std::less<pgn::cEntityWptr>::operator ()(const pgn::cEntityWptr &lhs ,const pgn::cEntityWptr &rhs) const {return *lhs.lock() < *rhs.lock();}
