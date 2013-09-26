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

		//! get shared pointer
		sptr_type sptr() const {return sptr_type(this);}
	private:
		friend class cEntityMgr;
		cEntity();
		~cEntity(){}

	private:
		boost::uuids::uuid mId;
    };
}