#pragma once

#include <core/util/json_conversions.h>

#include "ecs_config.h"
#include "Event.h"

namespace pgn
{
	DECL_EVENT(SystemActivity, bool);

	class cEntity;

	class cSystemBase
	{
		public:
			cSystemBase();
			virtual ~cSystemBase();
			virtual void Process()=0;

			virtual std::string to_string()  const=0;
			virtual void to_json(rapidjson::Value& zRoot) const =0;
			virtual void from_json(const rapidjson::Value& zRoot)=0;

			void SetActive(bool zActive);
			bool Active() const {return mActive;}
		protected:
			virtual void ProcessOne(const cEntity& zE)=0;

		private:
			bool mActive;
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
}