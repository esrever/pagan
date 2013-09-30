#pragma once

#include <iostream>

#include <core/util/conversions.h>
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

		virtual std::string to_string() const {return "BaseClass";}

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


inline std::ostream& operator<<(std::ostream& os,const pgn::cComponentBase& zComp)
{
	os<<zComp.to_string();
	return os;
}
namespace pgn{
inline std::string to_string( const cComponentBase& zCompo)
{
	std::ostringstream str;
	str << zCompo;
	return str.str();
}}