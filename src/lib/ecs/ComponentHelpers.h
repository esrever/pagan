#pragma once

#include "ecs_config.h"
#include "Component.h"

namespace pgn
{
	class cComponentBase;
	//! Is this a bad,costly paradigm?
	bool ComponentBelongsToEntity(const cComponentBase& zComponent, const cEntity& zEntity);

	//! Gets a component of an (entity,type) pair
	cComponentBaseSptr GetComponent( const cEntity& zEntity, const unsigned short zTypeIndex);

	template<class T> 
	bool GetComponent(const cEntity& zEntity, std::shared_ptr< cComponent<typename T> >& ptr);






	//-------------------------------------------------------------------------------------------------
	template<class T> 
	bool GetComponent(const cEntity& zEntity, std::shared_ptr< cComponent<typename T> >& ptr)
	{
		return false;
		//auto cptr =  GetComponent( zEntity, cComponent<T>::StaticTypeIndex() );
		//ptr = cptr;
		//return ptr != nullptr;
	}
}