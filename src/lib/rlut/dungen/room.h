#pragma once

#include "area.h"
#include "constraints.h"

namespace pgn
{
	namespace rlut
	{

		struct eRoomSideMask
		{
			static const int none = 0;
			static const int left = 1;
			static const int right = 2;
			static const int bottom = 4;
			static const int top = 8;

			static const int val2idx(int v);
		};

		struct cRoom : public cArea
		{
			cRoom(cWorkspace& ws, const glm::ivec2& start, const glm::ivec2& dims);

			typedef std::shared_ptr<cRoom> ptr_type;
			glm::ivec2 mStart;
			glm::ivec2 mDims;

			// utilities

			//! on which side does the perimeter point reside on?
			int which_side_mask(const glm::ivec2& ppt) const;
			int which_side_mask_idx(const glm::ivec2& ppt) const;

			// virtuals
			virtual void Init();
			virtual void update_perimeter();
			virtual void calc_avail_connection_slots();

			// creator functions
			static ptr_type create(cWorkspace& ws);
			static ptr_type create(cWorkspace& ws, const glm::ivec2& start);

			// utilities
			glm::ivec2 side_midpoint(int side) const;
		};

	}
}