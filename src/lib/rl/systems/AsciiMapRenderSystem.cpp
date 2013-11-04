#include "AsciiMapRenderSystem.h"

#include <curses.h>

#include <ecs/Component.h>
#include <ecs/EntityComponents.h>

#include <rl/components/AsciiWindow.h>

namespace pgn
{
	void cAsciiMapRenderSystem::Process()
	{
		ProcessQuery("component_GameLog");
	}

	void cAsciiMapRenderSystem::ProcessSingle(const std::map< cEntity, cEntityComponents>::const_iterator& zEc)
	{
		std::shared_ptr< cComponent<cAsciiWindow>> asciiwin_ptr;
		zEc->second.GetComponent(asciiwin_ptr);
	}

	bool cAsciiMapRenderSystem::from_json(const rapidjson::Value& zRoot)
	{
		return cSystemBase::from_json(zRoot);
	}
}