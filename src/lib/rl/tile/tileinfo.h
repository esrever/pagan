#pragma once

#include <ecs/ecs.h>

namespace pgn
{
	namespace rl
	{
		struct cTileInfo
		{
			cTileInfo();

			//! LightLevel: obtain from nearby torches, etc
			struct cGeneralInfo;

			//! required
			// floor, wall, etc
			// has default, overridable movecost
			cECS::cEntityWithDataConst mBg;

			//! optional
			// door,stairs,bridge, statue, chest, etc
			// has default, overridable movecost
			cECS::cEntityWithDataConst mFg;

			//! optional
			// mist, poison cloud, web, firewall, etc
			// has default, overridable movecost
			std::vector<cECS::cEntityWithDataConst> mAtmo;

			//! optional
			std::vector<cECS::cEntityWithDataConst> mItems;

			//! optional
			// has default, overridable movecost
			cECS::cEntityWithDataConst mActor;
		};
	}
}