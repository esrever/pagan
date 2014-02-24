#include "difi.h"

#include <algorithm>
#include <deque>

namespace pgn
{
	namespace rlut
	{
		template<class T>
		void cDiFi<T>::Init(const index2d_type& dims,
			norm_function_type norm_func)
		{
			mMap.Resize(dims.x, dims.y);
			assert(std::max(dims.x, dims.y) < std::numeric_limits<T>::max());
			mMinDifferenceHorz = norm_func(glm::ivec2(1, 0));
			mMinDifferenceDiag = norm_func(glm::ivec2(1, 1));
		}

		//---------------------------------------------------
		template<class T>
		void cDiFi<T>::Merge(const cDiFi<T>& inp,
			const glm::ivec2& rel_offset,
			std::function< T(const T&, const T&)> merge_func)
		{
			Merge(glm::ivec2(0, 0), inp, rel_offset, merge_func, *this);
		}

		//---------------------------------------------------
		template<class T>
		template<class U>
		void cDiFi<T>::Generate(const cArray2D<U>& input_map,				// is-obstacle or not, movecost
			const glm::ivec2&		   difi_start,				// lower-left point of difi map
			const std::vector<glm::ivec2>& goal_points,		// list of goal points to start expanding from, in input_map coordinates
			std::function<T(const U&)> convert_func)
		{
			cArray2D<bool> processed;
			processed.Resize(mMap.Width(), mMap.Height(), false);
			mMap.Fill(std::numeric_limits<T>::max());
			std::deque<glm::ivec2> stack_p;
			for (auto& p : goal_points)
			{
				stack_p.push_back(p - difi_start);
				assert(input_map.InRange(p));
			}
			std::deque<T> stack_v(goal_points.size(), T(0));
			const glm::ivec2 off_horz[] = { glm::ivec2(-1, 0), glm::ivec2(1, 0), glm::ivec2(0, -1), glm::ivec2(0, 1) };
			const glm::ivec2 off_diag[] = { glm::ivec2(-1, -1), glm::ivec2(1, -1), glm::ivec2(-1, 1), glm::ivec2(1, 1) };
			int iters = 0;
			int iters2 = 0;
			while (!stack_p.empty())
			{
				++iters;
				const auto p = stack_p.front();
				const auto v = stack_v.front();
				stack_p.pop_front();
				stack_v.pop_front();
				if (mMap.InRange(p))
				{
					bool prev_processed = processed(p);
					processed(p) = true;
					auto input_map_pt = p + difi_start;
					// !processed.get(p)
					if (((mMap(p) > v) || (!prev_processed)) && input_map.InRange(input_map_pt))
					{
						auto move_cost = convert_func(input_map(input_map_pt));
						auto vnew = move_cost != std::numeric_limits<T>::max() ? v* move_cost : move_cost;
						mMap(p) = std::min(vnew, std::numeric_limits<T>::max());
						if (mMap(p) != std::numeric_limits<T>::max())
						{
							++iters2;
							for (int i = 0; i < 4; ++i)
							{
								stack_p.push_back(p + off_horz[i]);
								stack_v.push_back(v + mMinDifferenceHorz);
							}
							for (int i = 0; i < 4; ++i)
							{
								stack_p.push_back(p + off_diag[i]);
								stack_v.push_back(v + mMinDifferenceDiag);
							}
						}
					}
				}
			}
			//while (iters == 123);
		}


		//---------------------------------------------------
		template<class T>
		template<class U>
		void cDiFi<T>::Merge(const glm::ivec2& rel_offset_this,
			const cDiFi<T>& inp,
			const glm::ivec2& rel_offset,
			std::function< U(const T&, const T&)> merge_func,
			cDiFi<U>& outp)
		{
			assert(false);
		}


		//--------------------------------------------------------------------------------------
		// Link templates

		template cDiFi<int>;
		template void cDiFi<int>::Generate<bool>(const cArray2D<bool>& input_map,
			const glm::ivec2&		   difi_start, const std::vector<glm::ivec2>& goal_points,
			std::function<int(const bool&)> convert_func);
		template void cDiFi<int>::Generate<int>(const cArray2D<int>& input_map,
			const glm::ivec2&		   difi_start, const std::vector<glm::ivec2>& goal_points,
			std::function<int(const int&)> convert_func);

	}
}

