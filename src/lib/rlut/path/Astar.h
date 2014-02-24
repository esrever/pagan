#pragma once

#include <deque>
#include <set>
#include <vector>

#include <core/container/Array2D.h>
#include <core/util/glm_utils.h>
#include <rlut/utils/shape/ShapeCalc.h>

/*
	Provide:
		
		movecost function:			float(ivec2, ivec2)  <-- GSCORE!
			blockage function?		movecost = max
		neighbour shape function	ShapeCalc
		max_score					1e5f good enough?
		search space:				for Fscore/Gscore resize
		single goal
			alpha						for Dijkstra/BFS
			goal-as-point:				ivec2
			heuristic function:			float(ivec2, ivec2)  <-- FSCORE - GSCORE!
		multi-goal
			goal-as-function:			bool(ivec2) <-- MULTIGOAL!


		How to merge single/multi goal?
			template search on goal functor
				Single goal
				Many goal
				arb function

			goal functor provides a heuristic

			struct cSingleGoal
				cSingleGoal( ivec2)
*/

namespace pgn
{
	namespace rlut
	{
		typedef std::function<float(const glm::ivec2&, const glm::ivec2&)> astar_norm_func_type;

		struct cSingleGoal
		{
			cSingleGoal(const glm::ivec2& g, astar_norm_func_type f) :mGoal(g), mNorm(f){}
			glm::ivec2 mGoal;
			astar_norm_func_type mNorm;

			bool IsGoal(const glm::ivec2& p) const
			{
				return mGoal == p;
			}

			float Heuristic(const glm::ivec2& p) const
			{
				return mNorm(p, mGoal);
			}
		};

		struct cMultiGoal
		{
			cMultiGoal(const std::vector<glm::ivec2>& g, astar_norm_func_type f) :mGoals(g), mNorm(f){}
			std::vector<glm::ivec2> mGoals;
			astar_norm_func_type mNorm;

			bool IsGoal(const glm::ivec2& p) const
			{
				for (const auto& g : mGoals)
				if (g == p)
					return true;
				return false;
			}

			float Heuristic(const glm::ivec2& p) const
			{
				float minv = std::numeric_limits<float>::max();
				for (const auto& g : mGoals)
					minv = std::min(minv, mNorm(p, g));
				return minv;
			}
		};

		struct cFunctionGoal
		{
			typedef std::function<float(const glm::ivec2&)> heuristic_func_type;
			typedef std::function<bool(const glm::ivec2&)> goal_func_type;
			cFunctionGoal(goal_func_type fg, heuristic_func_type fh) :mGoalFunc(fg), mHeuristicFunc(fh){}

			goal_func_type		mGoalFunc;
			heuristic_func_type mHeuristicFunc;

			bool IsGoal(const glm::ivec2& p) const
			{
				return mGoalFunc(p);
			}

			float Heuristic(const glm::ivec2& p) const
			{
				return mHeuristicFunc(p);
			}
		};

		class cAstar
		{
		public:
			typedef std::function<float(const glm::ivec2&, const glm::ivec2&)> movecost_func_type;

			//! Single goal search
			template<class T>
			bool Search(const glm::ivec2& start,
				const T& goal,
				std::deque<glm::ivec2>& path,
				const cArrayShape2D& searchSpace,
				movecost_func_type gfunc,
				const cShapeCalcTraits::ret_type& nbs,
				float alpha = 0.5f);

		private:
			void ReconstructPath(const glm::ivec2& start, const glm::ivec2& goal, std::deque<glm::ivec2>& path, const glm::ivec2& offset);
		private:
			cArray2D<float> mFScore;
			cArray2D<float> mGScore;
			std::vector<glm::ivec2>  mOpenSet;
			std::set<glm::ivec2> mVisitedSet;
			cArray2D<glm::ivec2> mLinks;
		};
	}
}