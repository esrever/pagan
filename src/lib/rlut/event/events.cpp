#include "events.h"

#include <core/util/string.h>
#include <rlut/components/components.h>
#include <rlut/fov/fov_rsc.h>

namespace pgn
{
	

	/*
	template<>
	bool evt::cPlayerAppear::Run(const ecs::cmp::cLocation& pos)
	{
		auto world = mainecs()->TagusToEntities("World")->second->second.Component<ecs::cmp::cWorldData>();
		auto ped = mainecs()->TagusToEntities("Player")->second;
		if (evt::cAppear::Run(ped, pos))
		{
			auto loc = ped->second.Component<ecs::cmp::cLocation>();
			// Set new current level
			mainecs()->Tagu("CurrentLevel", world->mLevelMap[loc->mLevelId]);
			return evt::cCalculateVisibility::Run(ped);
		}
		else
			return false;
	}

	template<>
	bool evt::cAppear::Run(ecs::cEntityWithData ed, const ecs::cmp::cLocation& pos)
	{
		// set location
		auto loc = ed->second.Component<ecs::cmp::cLocation>();
		*loc = pos;

		// set the actor in the level
		auto world = mainecs()->TagusToEntities("World")->second->second.Component<ecs::cmp::cWorldData>();
		auto lvl = world->mLevelMap[loc->mLevelId]->second.Component<ecs::cmp::cLevelData>();
		lvl->mLayout.SetActor(ed);
		return true;
	}

	template<>
	bool evt::cCreateLevel::Run(ecs::cEntityWithData ed)
	{
		// TODO: flesh out. Currently store the id -> level association
		auto level = ed->second.Component<ecs::cmp::cLevelData>();
		auto world = mainecs()->TagusToEntities("World");
		world->second->second.Component<ecs::cmp::cWorldData>()->mLevelMap[level->mId] = ed;
		return true;
	}
	*/
}