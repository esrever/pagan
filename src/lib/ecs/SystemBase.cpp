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
		EMIT_EVENT(SystemActivity, zActive);
	}
}