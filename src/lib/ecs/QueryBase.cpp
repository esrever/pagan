#pragma once

#include "QueryBase.h"

namespace pgn
{
	//-----------------------------------------------------------------
	cQueryBase cQueryBase::operator && (const cQueryBase &rhs) const
	{
		std::set<cEntity> eset;
		std::set_intersection(mEntities.begin(),mEntities.end(),rhs.Get().begin(),rhs.Get().end(),std::inserter(eset, eset.end()));
		return cQueryBase(eset);
	}

	//-----------------------------------------------------------------
	cQueryBase cQueryBase::operator || (const cQueryBase &rhs) const
	{
		std::set<cEntity> eset = mEntities;
		eset.insert(rhs.Get().begin(),rhs.Get().end());
		return cQueryBase(eset);
	}
}