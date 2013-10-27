#pragma once

#include <set>

#include <core/util/predicates.h>

#include "ecs_config.h"
#include "event.h"

namespace pgn
{
	//! Stores entities with at least the specified components. 
	class cQueryBase
	{
	public:
		//! ctors
		cQueryBase(){}
		cQueryBase(const std::set<cEntity>& zEnts):mEntities(zEnts){}
		virtual ~cQueryBase(){}

		//! Name
		virtual const std::string ReceiverName() const {return "Query";}

		//! Access data
		const std::set<cEntity>& Get() const {return mEntities;}

		virtual bool operator < (const cQueryBase &rhs) const {return container_hash<std::set<cEntity>>()(Get()) < container_hash<std::set<cEntity>>()(rhs.Get());}
		virtual bool operator == (const cQueryBase &rhs) const {return container_hash<std::set<cEntity>>()(Get()) == container_hash<std::set<cEntity>>()(rhs.Get());}

		virtual cQueryBase operator && (const cQueryBase &rhs) const;
		virtual cQueryBase operator || (const cQueryBase &rhs) const;

	protected:
		std::set<cEntity> mEntities;
	};
}