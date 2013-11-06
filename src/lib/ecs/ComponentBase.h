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
		//! Get type index(for masks), virtually
		virtual unsigned short TypeIndex() const =0;

		//! Json conversions
		virtual std::string to_string() const = 0;
		virtual void to_json(JsonWriter& zRoot) const = 0;//{}
		virtual bool from_json(const rapidjson::Value& zRoot)=0;//{}

	protected:
		cComponentBase(){}
	};

	DECL_PTRTYPE(cComponentBase)	

	//------------------------------------------------------------------------
	template<>
	void to_json<cComponentBase>(const cComponentBase& zCompo, JsonWriter& zRoot);
	//------------------------------------------------------------------------
	template<>
	bool from_json<cComponentBase>(cComponentBase& zCompo, const rapidjson::Value& zRoot);
	//------------------------------------------------------------------------
	template<>
	std::string to_string<cComponentBase>(const cComponentBase& zCompo);

}
