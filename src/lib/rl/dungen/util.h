#pragma once

#include <functional>
#include <set>
#include <glm/glm.hpp>

namespace pgn
{
	namespace rl
	{
		struct cWorkspace;
		
		//! MultiGoal functor
		struct cAnyGoalFirst
		{
			cAnyGoalFirst(const cWorkspace& ws, const glm::ivec2& start, size_t thisId, const std::set<size_t> excludeIds);

			bool IsGoal(const glm::ivec2& p) const;
			float Heuristic(const glm::ivec2& p) const	{ return 0.0f;	}

		private:
			const glm::ivec2& mStart;
			const cWorkspace& mWs;
			const size_t mStartId;
			const std::set<size_t> mExcludeIds;
		};

		//! movecost function generator
		std::function<float(const glm::ivec2&, const glm::ivec2&)> GenerateMoveCostFunction(const cWorkspace& ws, int invBits, int reqBits);
	}
}