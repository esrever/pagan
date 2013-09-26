#pragma once

#include <memory>
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
		typedef std::shared_ptr<const cEntity> sptr_type;
		typedef std::weak_ptr<const cEntity> wptr_type;

		//! get shared pointer
		sptr_type sptr() const {return sptr_type(this);}
		wptr_type wptr() const {return std::make_shared<cEntity>(*this);}
	private:
		friend class cEntityMgr;
		//sfriend class sptr_type;
		cEntity();

	private:
		boost::uuids::uuid mId;
    };

	typedef cEntity::wptr_type cEntityPtr;
	typedef cEntity::sptr_type cEntitySptr;
}

bool std::less<pgn::cEntityPtr>::operator ()(const pgn::cEntityPtr &lhs ,const pgn::cEntityPtr &rhs) const
{
	return lhs.lock() < rhs.lock(); 
}