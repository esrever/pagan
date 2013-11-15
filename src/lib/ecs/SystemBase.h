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
			cSystemBase();
			virtual ~cSystemBase();

			//! Process entities
			virtual void Process() = 0;

			//! Json related
			virtual void to_json(JsonWriter& zRoot) const;
			virtual bool from_json(const rapidjson::Value& zRoot);

			//! System activity
			void SetActive(bool zActive);
			bool Active() const {return mActive;}

			//! Pointer generator
			template<class T>
			static std::shared_ptr<cSystemBase> Create();

		protected:
			bool LoadQuery(pgn::cQueryExpressionSptr& zQuery, const rapidjson::Value& zRoot, const char * zName) const;

		protected:
			bool mActive;
			std::string mName;
			std::string mDesc;
	};

	typedef cEvent<size_t(eBasicECSEvent::SYSTEM_ACTIVE),  cSystemBase * , bool> cSystemActiveEvent;


	//--------------------------------------------------
	//! sys to json
	template<>
	void to_json<cSystemBase>(const cSystemBase& zSys, JsonWriter& zRoot);
	//! sys from json
	template<>
	bool from_json<cSystemBase>(cSystemBase& zSys, const rapidjson::Value& zRoot);
	//! sys to string
	template<>
	std::string to_string<cSystemBase>(const cSystemBase& zSys);

	template<class T>
	std::shared_ptr<cSystemBase> cSystemBase::Create()
	{
		return std::shared_ptr<cSystemBase>(new T());
	}

	/*
	template<class T>
	void ProcessQuery(const std::string& zQueryName, T * const zPtr, std::function<void(T * const, const std::map< cEntity, cEntityComponents>::const_iterator&)> func)
	{
		// Get the query. This should be easily retrievable
		auto qit = ECS.mSystemMgr->GetQueries().find(zQueryName);
		if (qit != ECS.mSystemMgr->GetQueries().end())
		{
			// Get entities found by the query
			auto ents = qit->second.Entities();
			for (auto e : ents)
			{
				// Get the entities' components
				auto ec = ECS.mEntityMgr->GetComponents().find(e);
				func(zPtr, ec);
			}
		}
	}
	*/
	
	
}