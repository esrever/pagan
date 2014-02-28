#pragma once

#include <iostream>

#include "ecs-config.h"

namespace pgn
{
	//! Class used for common storage.
	class cComponentBase
	{
		SUPPORT_DERIVED(cComponentBase)
	public:
		//! Get type index(for masks), virtually
		virtual unsigned short TypeIndex() const =0;

	protected:
		cComponentBase(){}
	};

	DECL_PTRTYPE(cComponentBase);
	DECL_SERIALIZE_INTERFACE(cComponentBase);

}
