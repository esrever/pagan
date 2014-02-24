#pragma once

#include <glm/glm.hpp>

#define DEF_FLUENT( T, V) T m##V; cConstraints& V##(const T & v) { m##V = v; return *this;}

namespace pgn
{
	namespace rlut
	{

		struct cConstraints
		{
			DEF_FLUENT(glm::ivec2, RoomRectW);
			DEF_FLUENT(glm::ivec2, RoomRectH);
			DEF_FLUENT(glm::ivec2, DoorToDoorMinDistance);
			DEF_FLUENT(glm::ivec2, DoorsPerRoom);
			DEF_FLUENT(bool, ConnectIslands);
			DEF_FLUENT(bool, StoreRoomConnectivity);
			DEF_FLUENT(bool, OverrideMaxConns);
			DEF_FLUENT(bool, RandomEntryExit);
		};
#undef DEF_FLUENT

		struct cArea;

		// utility functions:
		bool cons_door2door_min_distance_check(const cArea& area, const glm::ivec2& pt);
		bool is_perimeter_corner(const cArea& area, const glm::ivec2& pt);

	}
}