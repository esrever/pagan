#include "AsciiRenderSystem.h"

#include <curses.h>

#include <ecs/Component.h>
#include <ecs/EntityComponents.h>

#include <rl/components/AsciiRep.h>
#include <rl/components/Location.h>

namespace pgn
{
	void cAsciiRenderSystem::Process()
	{
		ProcessQuery("component_AsciiRep");
		// TODO: process subsystems one-by-one, set the order in the file. subsystems are the level render & entity render
		refresh();
	}

	void cAsciiRenderSystem::ProcessSingle(const std::map< cEntity, cEntityComponents>::const_iterator& zEc)
	{
		// Get AsciiRep & location components
		std::shared_ptr< cComponent<cAsciiRep>> ascii_ptr;
		std::shared_ptr< cComponent<cLocation>> loc_ptr;
		zEc->second.GetComponent( ascii_ptr);
		zEc->second.GetComponent( loc_ptr);

		// TODO: if location in current level,render
		
	}

	bool cAsciiRenderSystem::from_json(const rapidjson::Value& zRoot)
	{
		return cSystemBase::from_json(zRoot);
	}
}