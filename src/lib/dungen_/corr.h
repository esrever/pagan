#pragma once

#include <deque>

#include "area.h"
#include "constraints.h"

namespace pgn
{
	namespace dungen
	{

		struct cCorr : public cArea
		{
			cCorr(cWorkspace& ws, const std::deque<glm::ivec2>& pts);

			typedef std::shared_ptr<cCorr> ptr_type;

			// I could use these in order! T-junctions are different corridors
			std::vector<glm::ivec2> mPts;

			// virtuals
			virtual void Init();
			virtual void update_perimeter();
			virtual void calc_avail_connection_slots();

		};

	}
}