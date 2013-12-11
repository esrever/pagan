#pragma once

#include <string>
#include <memory>
#include <core/util/json_conversions.h>

#include "QueryExpression.h

namespace pgn
{
	//! Base class for a system. 
	class cSystemBase
	{
		public:
			//! ctors
			cSystemBase();
			virtual ~cSystemBase();

			//! Pointer generator
			template<class T>
			static std::shared_ptr<cSystemBase> Create();

		private:
			bool LoadQuery(pgn::cQueryExpressionSptr& zQuery, const rapidjson::Value& zRoot, const char * zName) const;

		protected:
			std::string mName;
			std::string mDesc;
	};

	//--------------------------------------------------
	//! sys to json
	template<>
	void to_json<cSystemBase>(const cSystemBase& zSys, JsonWriter& zRoot);
	//! sys from json
	template<>
	bool from_json<cSystemBase>(cSystemBase& zSys, const rapidjson::Value& zRoot);

	template<class T>
	std::shared_ptr<cSystemBase> cSystemBase::Create()
	{
		return std::shared_ptr<cSystemBase>(new T());
	}	
}

//------------------------------------------------------
#pragma once
#include <ecs/SystemBase.h>
#include <rl/events/events.h>

namespace pgn
{
	namespace sys
	{
		class c$(name) : public cSystemBase
		{
		public:
			c$(name):mProcess(){}
		private:
			cEventHandler<evt::$(ecc)> mProcess;
			void Process( $(args));
			
		};
	}
}