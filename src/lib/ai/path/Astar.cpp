#include "Astar.h"

#include <core/math/norm.h>
#include <core/container/Array2D_utils.h>

namespace pgn
{
	namespace ai
	{
		//! Single goal search
		template<class T>
		bool cAstar::Search(const glm::ivec2& start0,
			const T& fgoal,
			std::deque<glm::ivec2>& path,
			const cArrayShape2D& searchSpace,
			movecost_func_type gfunc,
			const cShapeCalcTraits::ret_type& nbs,
			float alpha)
		{
			// 1st goal check
			if (fgoal.IsGoal(start0))
			{
				path.clear();
				path.push_back(start0);
				return true;
			}

			// Resize if necessary
			if ((searchSpace.Width() > mGScore.Width()) || (searchSpace.Height() > mGScore.Height()))
			{
				mFScore.Resize(searchSpace.Width(), searchSpace.Height(), std::numeric_limits<float>::max());
				mGScore.Resize(searchSpace.Width(), searchSpace.Height(), std::numeric_limits<float>::max());
				mLinks.Resize(searchSpace.Width(), searchSpace.Height());
			}

			glm::ivec2 offFGL(searchSpace.RealX(), searchSpace.RealY());

			glm::ivec2 start = start0 - offFGL;

			// Clear and initialize with start
			mOpenSet.clear();
			mOpenSet.push_back(start);
			mVisitedSet.clear();
			mVisitedSet.insert(start);

			const float alphaDenom = 1.0f / alpha;

			mGScore(start) = 0;
			mFScore(start) = (1 - alpha)*fgoal.Heuristic(start + offFGL)* alphaDenom;

			// main loop
			bool res = false;
			while (!mOpenSet.empty())
			{
				// find smallest Fscore node in open set, keep and remove it
				auto best_it = mOpenSet.begin();
				auto cmp = Array2D_compare_less<float>(mFScore);
				for (auto it = mOpenSet.begin() + 1; it != mOpenSet.end(); ++it)
				{
					if (cmp(*it, *best_it))
						best_it = it;
				}
				auto current = *best_it;
				mOpenSet.erase(best_it);
				auto g_cur = mGScore(current);

				for (auto nbit = nbs.first; nbit != nbs.second; ++nbit)
				{
					glm::ivec2 nb = current + *nbit;
					if (searchSpace.InRange(nb.x, nb.y))
					{
						if (fgoal.IsGoal(nb + offFGL))
						{
							mLinks(nb) = current;
							ReconstructPath(start, nb, path, offFGL);
							res = true;
							break;
						}
						else
						{
							auto add_gscore = gfunc(current + offFGL, nb + offFGL);
							if (add_gscore != std::numeric_limits<float>::max())
							{
								// calc score estimate
								auto tentative_g_score = g_cur + add_gscore;
								// if better than existing
								if (tentative_g_score < mGScore(nb))
								{
									// store data
									mVisitedSet.insert(nb);
									mLinks(nb) = current;
									mGScore(nb) = tentative_g_score;
									mFScore(nb) = (alpha*tentative_g_score + (1 - alpha)*fgoal.Heuristic(nb + offFGL))*alphaDenom;

									// add to openset if not there. TODO: assert that it's always end?
									if (std::find(mOpenSet.begin(), mOpenSet.end(), nb) == mOpenSet.end())
										mOpenSet.push_back(nb);
								}
							}
						}
					}
				}
				if (res)
					break;
			}

			// for each visited node, reinitialize
			for (const auto& v : mVisitedSet)
			{
				mFScore(v) = std::numeric_limits<float>::max();
				mGScore(v) = std::numeric_limits<float>::max();
			}
			return res;
		}

		/*
	//! Multi-goal search
	bool cAstar::Search(const glm::ivec2& start0,
	goal_func_type goalfunc,
	std::deque<glm::ivec2>& path,
	const cArrayShape2D& searchSpace,
	movecost_func_type gfunc,
	const cShapeCalcTraits::ret_type& nbs)
	{
	// 1st goal check
	if (goalfunc(start0))
	{
	path.clear();
	path.push_back(start0);
	return true;
	}

	// Resize if necessary
	if ((searchSpace.Width() > mGScore.Width()) || (searchSpace.Height() > mGScore.Height()))
	{
	mFScore.Resize(searchSpace.Width(), searchSpace.Height(), std::numeric_limits<float>::max());
	mGScore.Resize(searchSpace.Width(), searchSpace.Height(), std::numeric_limits<float>::max());
	mLinks.Resize(searchSpace.Width(), searchSpace.Height());
	}


	glm::ivec2 offFGL(searchSpace.RealX(), searchSpace.RealY());
	glm::ivec2 start = start0 - offFGL;

	// Clear and initialize with start
	mOpenSet.clear();
	mOpenSet.push_back(start);
	mVisitedSet.clear();
	mVisitedSet.insert(start);

	mGScore(start) = 0;

	// main loop
	bool res = false;
	while (!mOpenSet.empty())
	{
	// find smallest Fscore node in open set, keep and remove it
	auto best_it = mOpenSet.begin();
	auto cmp = Array2D_compare_less<float>(mGScore);
	for (auto it = mOpenSet.begin() + 1; it != mOpenSet.end(); ++it)
	{
	if (cmp(*it, *best_it))
	best_it = it;
	}
	auto current = *best_it;
	mOpenSet.erase(best_it);
	auto g_cur = mGScore(current);

	for (auto nbit = nbs.first; nbit != nbs.second; ++nbit)
	{
	glm::ivec2 nb = current + *nbit;
	if (searchSpace.InRange(nb.x, nb.y))
	{
	if (goalfunc(nb+offFGL))
	{
	mLinks(nb) = current;
	ReconstructPath(start, nb, path, offFGL);
	res = true;
	break;
	}
	else
	{
	auto add_gscore = gfunc(current + offFGL, nb + offFGL);
	if (add_gscore != std::numeric_limits<float>::max())
	{
	// calc score estimate
	auto tentative_g_score = g_cur + add_gscore;
	// if better than existing
	if (tentative_g_score < mGScore(nb))
	{
	// store data
	mVisitedSet.insert(nb);
	mLinks(nb) = current;
	mGScore(nb) = tentative_g_score;

	// add to openset if not there. TODO: assert that it's always end?
	if (std::find(mOpenSet.begin(), mOpenSet.end(), nb) == mOpenSet.end())
	mOpenSet.push_back(nb);
	}
	}
	}
	}
	}
	if (res)
	break;
	}

	// for each visited node, reinitialize
	for (const auto& v : mVisitedSet)
	{
	mFScore(v) = 1e5f;
	mGScore(v) = 1e5f;
	}
	return res;
	}

	*/

		void cAstar::ReconstructPath(const glm::ivec2& start, const glm::ivec2& goal, std::deque<glm::ivec2>& path, const glm::ivec2& offset)
		{
			path.clear();
			path.push_front(goal + offset);
			path.push_front(mLinks(goal) + offset);
			glm::ivec2 start1 = start + offset;
			while (path.front() != start1)
				path.push_front(mLinks(path.front() - offset) + offset);
		}

		template bool cAstar::Search<cSingleGoal>(const glm::ivec2& start0,
			const cSingleGoal& goal0,
			std::deque<glm::ivec2>& path,
			const cArrayShape2D& searchSpace,
			movecost_func_type gfunc,
			const cShapeCalcTraits::ret_type& nbs,
			float alpha);

		template bool cAstar::Search<cMultiGoal>(const glm::ivec2& start0,
			const cMultiGoal& goal0,
			std::deque<glm::ivec2>& path,
			const cArrayShape2D& searchSpace,
			movecost_func_type gfunc,
			const cShapeCalcTraits::ret_type& nbs,
			float alpha);

		template bool cAstar::Search<cFunctionGoal>(const glm::ivec2& start0,
			const cFunctionGoal& goal0,
			std::deque<glm::ivec2>& path,
			const cArrayShape2D& searchSpace,
			movecost_func_type gfunc,
			const cShapeCalcTraits::ret_type& nbs,
			float alpha);
	}
}