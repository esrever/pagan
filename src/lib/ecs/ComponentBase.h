#pragma once

#include <iostream>

#include <core/util/conversions.h>
#include <core/util/json_conversions.h>
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

		virtual std::string to_string() const {return "cComponentBase";}
		virtual void to_json(rapidjson::Value& zRoot) const{}
		virtual void from_json(const rapidjson::Value& zRoot){}
	protected:
		cComponentBase(){}
	};

	DECL_PTRTYPE(cComponentBase)	

	//! sys to json
	template<>
	void to_json<cComponentBase>(const cComponentBase& zCompo, rapidjson::Value& zRoot);
	//! sys from json
	template<>
	void from_json<cComponentBase>(cComponentBase& zCompo, const rapidjson::Value& zRoot);
	//! sys to string
	template<>
	std::string to_string<cComponentBase>(const cComponentBase& zCompo);
}
