#pragma once

#include <ecs/ecs.h>

namespace pgn
{
	struct cTileInfo
	{
		//! LightLevel: obtain from nearby torches, etc
		struct cGeneralInfo;

		//! required
		// floor, wall, etc
		// has default, overridable movecost
		cECS::cEntityWithData mBg;

		//! optional
		// door,stairs,bridge, statue, chest, etc
		// has default, overridable movecost
		cECS::cEntityWithData mFg;

		//! optional
		// mist, poison cloud, web, firewall, etc
		// has default, overridable movecost
		std::vector<cECS::cEntityWithData> mAtmo;

		//! optional
		std::vector<cECS::cEntityWithData> mItems;

		//! optional
		// has default, overridable movecost
		cECS::cEntityWithData mActor;
	};
}