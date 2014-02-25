#pragma once

#include <functional>

#include <core/container/Array2D.h>

namespace pgn
{
	namespace rlut
	{
		/*
			Do it like in functional goal: 
				Start from the goal points
				Expand to the rest
			Other ops:
				impl weighted difi combination
				use existing facilities for arithmetic ops?
		*/
		class cDiFi
		{
		public:
			typedef float																data_type;
			typedef glm::ivec2															index2d_type;
			typedef std::function<data_type(const index2d_type&, const index2d_type&)>  movecost_func_type;
			typedef cArray2D<data_type>													array2d_type;
			typedef std::function< data_type(const index2d_type&)>						norm_function_type;

		public:
			//! only provide dimensions for the distance field
			void Init(const index2d_type& dims);

			//! goals and move function are given in RELATIVE coordinates to difi!
			void Generate(movecost_func_type f, const glm::ivec2& corner, const glm::ivec2& goal);
			void Generate(movecost_func_type f, const glm::ivec2& corner, const std::vector<glm::ivec2>& goals);

		public:
			array2d_type& Data() { return mMap; }
			const glm::ivec2& CornerWcs() const { return mCornerWcs; }

		private:
			array2d_type	mMap;
			glm::ivec2		mCornerWcs;
		};
	}
}


//------------------------------------------------------------------------