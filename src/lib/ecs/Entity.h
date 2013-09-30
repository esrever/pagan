#pragma once

#include <bitset>
#include <boost/uuid/uuid.hpp>        
#include <boost/uuid/uuid_generators.hpp>

#include "Event.h"
#include "ecs_config.h"

namespace pgn 
{
	class cEntityMgr;

    //! 
    class cEntity
    {
	public:
		const boost::uuids::uuid& Id() const {return mId;} 


		bool operator < (const cEntity& e) const {return mId < e.Id();}
		std::ostream& operator<<(std::ostream& os) const
		{
			os<<Id().data;
		  return os;
		}
	private:
		friend class cEntityMgr;
		//sfriend class sptr_type;
		cEntity();

	private:
		boost::uuids::uuid mId;
    };

	DECL_PTRTYPE(cEntity)

	DECL_EVENT(EntityCreated, cEntityWptr);
	DECL_EVENT(DestroyEntity, cEntityWptr);
}

bool std::less<pgn::cEntityWptr>::operator ()(const pgn::cEntityWptr &lhs ,const pgn::cEntityWptr &rhs) const
{
	return *lhs.lock() < *rhs.lock(); 
}