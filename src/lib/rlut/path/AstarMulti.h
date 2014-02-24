#pragma once

#include <deque>
#include <set>

#include <core/container/Array2D.h>
#include <core/util/glm_utils.h>

namespace pgn
{
	class cAstarMulti
	{
	public:
		cAstarMulti(const cArray2D<T>& mask);
		void Init();
		bool Search(const glm::ivec2& start, size_t startId, std::deque<glm::ivec2>& path, const std::set<size_t>& excludeIds = std::set<size_t>());
	private:
		bool IsSolution(const glm::ivec2& start, const glm::ivec2& pt, size_t start_id, const std::set<size_t>& excludeIds);
		void ReconstructPath(const glm::ivec2& start, const glm::ivec2& goal, std::deque<glm::ivec2>& path);
	private:
		const cArray2D<T>& mMask;
		cArray2D<float> mGScore;
		std::vector<glm::ivec2>  mOpenSet;
		ivec2_set mVisitedSet;
		std::vector<glm::ivec2> mVisitedSet2;
		cArray2D<glm::ivec2> mLinks;
	};

}