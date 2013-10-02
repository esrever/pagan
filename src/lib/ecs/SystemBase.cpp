#include "SystemBase.h"
#include "Event.h"

namespace pgn
{
	//---------------------------------------------------------------
	cSystemBase::cSystemBase()
		:mActive(false)
	{}

	//---------------------------------------------------------------
	cSystemBase::~cSystemBase()
	{
		SetActive(false);
	}
	
	//---------------------------------------------------------------
	void cSystemBase::SetActive(bool zActive)
	{
		cSystemActivityEventData::emit(zActive);
	}

	//---------------------------------------------------------------
	template<>
	void to_json<cSystemBase>(const cSystemBase& zSys, rapidjson::Value& zRoot) 
	{
		zSys.to_json(zRoot);
	}

	//---------------------------------------------------------------
	template<>
	void from_json<cSystemBase>(cSystemBase& zSys, const rapidjson::Value& zRoot) 
	{
		zSys.from_json(zRoot);
	}

	//---------------------------------------------------------------
	template<>
	std::string to_string<cSystemBase>(const cSystemBase& zSys) 
	{
		rapidjson::Value root;
		zSys.to_json(root);
		return to_string(root);
	}
}