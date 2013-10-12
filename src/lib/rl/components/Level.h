#pragma once

#include <memory>
#include <core/container/Array2D.h>
#include <ecs/Event.h>
#include <rl/events/events.h>

namespace pgn
{
	class cEntity;
	//! Level component: default tiles, AI, sim.
	// TODO: listens to events that modify floor tiles. Initializes from sources such as bitmap, string, random, etc
	struct cLevel
	{
		cArray2D< std::weak_ptr<cEntity> > mFloorMap;

		//! Default tiles: shared so that they cannot be deleted
		std::shared_ptr<cEntity> mDefaultTile; 
		std::shared_ptr<cEntity> mDefaultAtmo;
	};
}