#include <rl/path/Astar.h>

int main()
{
	pgn::rl::cAstar astar;

	auto normfunc = [](const glm::ivec2& lhs, const glm::ivec2& rhs){ return float(pgn::norm_2(lhs - rhs)); };
	auto gfunc = [](const glm::ivec2& lhs, const glm::ivec2& rhs){ return float(pgn::norm_2(lhs - rhs)); };

	auto nbs8 = pgn::rl::cShapeCalc<pgn::rl::cBoxDistance>::Get(1, 1);
	auto nbs4 = pgn::rl::cShapeCalc<pgn::rl::cDiamondDistance>::Get(1, 1);


	// Try a single goal
	pgn::rl::cSingleGoal sgoal(glm::ivec2(5, 6), normfunc);
	std::deque<glm::ivec2> path;
	astar.Search(glm::ivec2(2, 2),
				 sgoal,
				 path,
				 pgn::cArrayShape2D(5,5,2,2),
				 gfunc,
				 nbs4);

	// Try a single goal, one obstacle
	pgn::rl::cSingleGoal sgoal2(glm::ivec2(5, 5), normfunc);
	auto gfunc2 = [](const glm::ivec2& lhs, const glm::ivec2& rhs){ return rhs == glm::ivec2(4, 4)
		? std::numeric_limits<float>::max()
		: float(pgn::norm_2(lhs - rhs)); };
	astar.Search(glm::ivec2(2, 2),
		sgoal2,
		path,
		pgn::cArrayShape2D(5, 5, 2, 2),
		gfunc2,
		nbs8);

	// Try multigoal
	std::vector<glm::ivec2> mgoal_pts;
	mgoal_pts.push_back(glm::ivec2(5, 6));
	mgoal_pts.push_back(glm::ivec2(0, 0));
	pgn::rl::cMultiGoal mgoal(mgoal_pts, normfunc);

	astar.Search(glm::ivec2(2, 2),
		mgoal,
		path,
		pgn::cArrayShape2D(10, 10, 0, 0),
		gfunc,
		nbs4);

	return 0;
}