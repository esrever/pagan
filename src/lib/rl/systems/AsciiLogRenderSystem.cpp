#include "AsciiLogRenderSystem.h"

#include <curses.h>

#include <ecs/Component.h>
#include <ecs/EntityComponents.h>

#include <rl/components/AsciiWindow.h>
#include <rl/components/GameLog.h>

namespace pgn
{
	void cAsciiLogRenderSystem::Process()
	{
		ProcessQuery("component_GameLog");
	}

	void cAsciiLogRenderSystem::ProcessSingle(const std::map< cEntity, cEntityComponents>::const_iterator& zEc)
	{
		std::shared_ptr< cComponent<cAsciiWindow>> asciiwin_ptr;
		std::shared_ptr< cComponent<cGameLog>> log_ptr;
		zEc->second.GetComponent(asciiwin_ptr);
		zEc->second.GetComponent(log_ptr);
	}

	bool cAsciiLogRenderSystem::from_json(const rapidjson::Value& zRoot)
	{
		return cSystemBase::from_json(zRoot);
	}
}