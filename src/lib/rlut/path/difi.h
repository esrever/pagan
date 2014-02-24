#pragma once

#include <functional>

#include <core/container/Array2D.h>

namespace pgn
{
	namespace rlut
	{
		template<class T>
		class cDiFi
		{
		public:
			typedef T									data_type;
			typedef cArray2D<T>							array2d_type;
			typedef glm::ivec2							index2d_type;
			typedef std::function<T(const glm::ivec2&)> norm_function_type;

		public:
			void Init(const index2d_type& dims,
				norm_function_type norm_func);

			template<class U>
			void Generate(const cArray2D<U>& input_map,				// is-obstacle or not, movecost
				const glm::ivec2&		   difi_start,				// lower-left point of difi map
				const std::vector<glm::ivec2>& goal_points,		// list of goal points to start expanding from
				std::function<T(const U&)> convert_func);			// how to convert from the input map to difi map score

			template<class U>
			void Merge(const glm::ivec2& rel_offset_this,
				const cDiFi<T>& inp,
				const glm::ivec2& rel_offset,
				std::function< U(const T&, const T&)> merge_func,
				cDiFi<U>& outp);

			void Merge(const cDiFi<T>& inp,
				const glm::ivec2& rel_offset,
				std::function< T(const T&, const T&)> merge_func);

		public:
			array2d_type& Data() { return mMap; }

		private:
			array2d_type	mMap;
			data_type		mMinDifferenceHorz;
			data_type		mMinDifferenceDiag;
		};
	}
}


//------------------------------------------------------------------------