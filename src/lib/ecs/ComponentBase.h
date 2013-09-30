#pragma once

#include <iostream>

#include "ecs_config.h"

namespace pgn
{
	//! Class used for common storage.
	class cComponentBase
	{
	public:
		virtual ~cComponentBase(){}
		//! Get the component type index 
		virtual unsigned short TypeIndex() const {return 0xFFFF;}

		/* stream-style

			virtual std::ostream& operator << (std::ostream& o) const;
			virtual std::istream& operator >> (std::istream& i) const;

			config-style

			ReadJson, WriteJson( document& doc)
		*/
	protected:
		cComponentBase(){}
	};

	DECL_PTRTYPE(cComponentBase)
}