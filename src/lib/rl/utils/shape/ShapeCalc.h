#pragma once

#include <vector>

#include <core/math/norm.h>

namespace pgn
{
	namespace rl
	{
		struct cBoxDistance { size_t operator ()(const glm::ivec2& v) const { return norm_inf(v); } };
		struct cDiskDistance { size_t operator ()(const glm::ivec2& v) const { return size_t(norm_2(v)); } };
		struct cDiamondDistance { size_t operator ()(const glm::ivec2& v) const { return norm_1(v); } };

		struct cShapeCalcTraits
		{
			typedef std::vector<glm::ivec2>			data_type;
			typedef data_type::const_iterator		iter_type;
			typedef std::pair<iter_type, iter_type>	ret_type;
		};

		//! Calculate a set of points that lie in a given shape
		template<class T>
		class cShapeCalc
		{
		public:
			static cShapeCalcTraits::ret_type Get(size_t rin, size_t rout);
		private:
			static void CalcRads(size_t r);
		private:
			static cShapeCalcTraits::data_type mData;
			static std::vector<size_t> mNextIdx;
		};
		template<class T>
		typename cShapeCalcTraits::data_type cShapeCalc<T>::mData;
		template<class T>
		std::vector<size_t> cShapeCalc<T>::mNextIdx;

		template<class T>
		typename cShapeCalcTraits::ret_type cShapeCalc<T>::Get(size_t rin, size_t rout)
		{
			assert(rout >= rin);
			if (rout >= mNextIdx.size())
				CalcRads(rout);
			auto s_idx = rin ? mNextIdx[rin - 1] : 0;
			auto e_idx = mNextIdx[rout];

			return std::make_pair(mData.begin() + s_idx, mData.begin() + e_idx);
		}

		template<class T>
		void cShapeCalc<T>::CalcRads(size_t r)
		{
			T dfunc;
			std::vector<std::vector<glm::ivec2>> radData(r + 1);
			size_t total = 0;
			int rs = 0;
			int re = int(r);
			for (int i = -re; i <= re; ++i)
			{
				for (int j = -re; j <= re; ++j)
				{
					const auto p = glm::ivec2(j, i);
					auto d = dfunc(p);
					if (d <= r)
					{
						radData[d].push_back(p);
						++total;
					}
				}
			}
			mNextIdx.resize(r + 1);
			mData.resize(total);
			for (size_t i = 0; i <= r; ++i)
			{
				size_t start_off = i ? mNextIdx[i - 1] : 0;
				mNextIdx[i] = radData[i].size() + start_off;
				std::copy(radData[i].begin(), radData[i].end(), mData.begin() + start_off);
			}
		}
	}
}