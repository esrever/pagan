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
	class cQueryBase;
	class cEntityComponents;

	//! Base class for a system. 
	class cSystemBase
	{
		public:
			//! ctors
			cSystemBase();
			virtual ~cSystemBase();

			//! Process entities
			virtual void Process(){};

			//! Processes a single entity-component
			virtual void ProcessSingle(const std::map< cEntity, cEntityComponents>::const_iterator& zEc){};

			void ProcessQuery(const std::string& zQueryName);

			//! Json related
			virtual void to_json(rapidjson::Value& zRoot) const{};
			virtual bool from_json(const rapidjson::Value& zRoot);

			//! System activity
			void SetActive(bool zActive);
			bool Active() const {return mActive;}

			//! Pointer generator
			template<class T>
			static std::shared_ptr<cSystemBase> Create();

		protected:
			bool mActive;
			std::string mName;
			std::string mDesc;
			std::vector< std::shared_ptr< cQueryBase> > mReferencedQueries;
	};

	typedef cEvent<size_t(eBasicECSEvent::SYSTEM_ACTIVE),  cSystemBase * , bool> cSystemActiveEvent;

	//! sys to json
	template<>
	void to_json<cSystemBase>(const cSystemBase& zSys, rapidjson::Value& zRoot);
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
	
}