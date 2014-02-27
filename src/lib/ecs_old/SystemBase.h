#pragma once

#include <vector>
#include <map>
#include <string>
#include <memory>
#include <core/util/json_conversions.h>

#include "ecs.h"
#include "ecs_config.h"
#include "Event.h"

namespace pgn
{
	class cEntityComponents;

	//! Base class for a system. 
	class cSystemBase
	{
		public:
			//! ctors
			virtual ~cSystemBase(){}

			//! Pointer generator
			template<class T>
			static std::shared_ptr<cSystemBase> Create();

		protected:
			bool LoadQuery(pgn::cQueryExpressionSptr& zQuery, const rapidjson::Value& zRoot, const char * zName) const;

		protected:
			DECL_JSON_PGN_FRIEND

			std::string mName;
			std::string mDesc;
	};

	//--------------------------------------------------
	DECL_JSON_PGN(cSystemBase)

	template<class T>
	std::shared_ptr<cSystemBase> cSystemBase::Create()
	{
		return std::shared_ptr<cSystemBase>(new T());
	}
}