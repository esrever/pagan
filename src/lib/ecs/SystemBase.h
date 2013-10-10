#pragma once

#include <vector>
#include <map>
#include <string>
#include <memory>
#include <core/util/json_conversions.h>

#include "ecs_config.h"
#include "Event.h"

namespace pgn
{
	DECL_EVENT(SystemActivity, bool);

	class cEntity;
	class cComponentQuery;

	//! Base class for a system. 
	class cSystemBase
	{
		public:
			cSystemBase();
			virtual ~cSystemBase();
			virtual void Process(){};

			virtual void to_json(rapidjson::Value& zRoot) const{};
			virtual void from_json(const rapidjson::Value& zRoot);

			void SetActive(bool zActive);
			bool Active() const {return mActive;}

			template<class T>
			static std::shared_ptr<cSystemBase> Create();
		protected:
			virtual void ProcessSingle(const cEntity& zE){};

		protected:
			bool mActive;
			std::string mName;
			std::string mDesc;
			std::vector< std::shared_ptr< cComponentQuery> > mReferencedQueries;
	};

	//! sys to json
	template<>
	void to_json<cSystemBase>(const cSystemBase& zSys, rapidjson::Value& zRoot);
	//! sys from json
	template<>
	void from_json<cSystemBase>(cSystemBase& zSys, const rapidjson::Value& zRoot);
	//! sys to string
	template<>
	std::string to_string<cSystemBase>(const cSystemBase& zSys);

	template<class T>
	std::shared_ptr<cSystemBase> cSystemBase::Create()
	{
		return std::shared_ptr<cSystemBase>(new T());
	}
	
}