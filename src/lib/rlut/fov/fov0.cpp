#include "fov0.h"

#include <core/math/norm.h>

namespace pgn
{
	namespace rlut
	{

		static void bresenham_line(const glm::ivec2& start, const glm::ivec2& end, std::vector<glm::ivec2>& pts)
		{
			auto dxy = glm::abs(end - start);
			glm::ivec2 sxy;
			sxy.x = start.x < end.x ? 1 : -1;
			sxy.y = start.y < end.y ? 1 : -1;
			auto err = dxy.x - dxy.y;

			pts.clear();
			auto pt = start;
			while (true)
			{
				pts.push_back(pt);
				if (pt == end) break;
				auto e2 = 2 * err;
				if (e2 > -dxy.y)
				{
					err -= dxy.y;
					pt.x += sxy.x;
				}
				if (pt == end)
				{
					pts.push_back(pt);
					break;
				}
				if (e2 < dxy.x)
				{
					err += dxy.x;
					pt.y += sxy.y;
				}
			}
		}

		static void bresenham_circle(int r, std::vector<glm::ivec2>& pts)
		{
			int x = r, y = 0;
			int radiusError = 1 - x;

			pts.clear();
			while (x >= y)
			{

				pts.push_back(glm::ivec2(x, y));
				pts.push_back(glm::ivec2(y, x));
				pts.push_back(glm::ivec2(-x, y));
				pts.push_back(glm::ivec2(-y, x));
				pts.push_back(glm::ivec2(-x, -y));
				pts.push_back(glm::ivec2(-y, -x));
				pts.push_back(glm::ivec2(x, -y));
				pts.push_back(glm::ivec2(y, -x));

				y++;
				if (radiusError < 0)
					radiusError += 2 * y + 1;
				else
				{
					x--;
					radiusError += 2 * (y - x + 1);
				}
			}
		}

		static void diamond(int r, std::vector<glm::ivec2>& pts)
		{
			pts.resize(4 * r);
			for (int xo = 0; xo < r; ++xo)
			{
				auto * p = &pts.at(4 * xo);
				*p = glm::ivec2(xo, r - xo); ++p;
				*p = glm::ivec2(r - xo, -xo); ++p;
				*p = glm::ivec2(-xo, xo - r); ++p;
				*p = glm::ivec2(xo - r, xo); ++p;
			}
		}

		static void diamond_full(int r, std::vector<glm::ivec2>& pts)
		{
			pts.clear();
			for (int i = -r; i <= r; ++i)
			{
				auto jbnd = r - abs(i);
				for (int j = -jbnd; j <= jbnd; ++j)
				{
					if (i | j)
					{
						auto nb = glm::ivec2(j, i);
						pts.push_back(nb);
					}
				}
			}
		}

		static float distance_point_to_line(const glm::ivec2& pt, const glm::ivec2& l0i, const glm::ivec2& l1i)
		{
			glm::vec2 p(pt.x, pt.y);
			glm::vec2 l0(l0i.x, l0i.y);
			glm::vec2 l1(l1i.x, l1i.y);

			const auto l01 = l1 - l0;
			const auto line_len_sq = glm::dot(l01, l01);
			const auto vp = p - l0;
			const auto vw = l01;
			const float t = glm::dot(vp, vw) / float(line_len_sq);

			auto proj = glm::vec2(l0 + t * vw);
			return glm::distance(p, proj);
		}

		//-----------------------------------------------------------------------------------
		void cFov0::Init(size_t los)
		{
			mLoS = los;

			// init vismap array (TODO: share later on, resize to larger if necessary, always center though! so check array size and los to determine starting point)
			mVisMap.Resize(los * 2 + 1, los * 2 + 1);
			mVisMap.Fill(0.0f);
			mVisMap(los, los) = 1.0f;
			mBlockMap.Resize(los * 2 + 1, los * 2 + 1);
			mBlockMap.Fill(false);

			//auto iters = rlut::cShapeCalc<rlut::cShapeDisk>::Get(1, los);
			//for (auto it = iters.first; it != iters.second; ++it)
			//	 mTierMap[it->x*it->x + it->y*it->y].push_back(*it);

			int ilos = int(los);
			// create tiers 
			for (int i = -ilos; i <= ilos; ++i)
			{
				for (int j = -ilos; j <= ilos; ++j)
				{
					if (i | j)
					{
						glm::ivec2 p(j, i);
						if (pgn::norm_2(p) <= float(los)) // TODO: different shape possible!
						{
							auto sqrd = pgn::norm_2(p);
							mTierMap[unsigned(sqrd)].push_back(p);
						}
					}
				}
			}

		}

		static float calc_vis(const cArray2D<bool>& dmap, const glm::ivec2&p, const glm::ivec2& off, const cArray2D<float>& visf, size_t los)
		{
			auto vcur = visf(off.x + los, off.y + los);
			auto bcur = dmap(p + off) ? 1.0f : 0.0f;
			auto curvis = vcur * bcur;
			return curvis;
		}

		void cFov0::Calc(const glm::ivec2& p, const cArray2D<bool>& vismap, std::vector<glm::ivec2>& lospts, cArray2D<float>& vis)
		{
			// for each tier
			lospts.clear();
			lospts.push_back(p);
			for (const auto& tier_keyval : mTierMap)
			{
				// for each tier element
				const auto& tier = tier_keyval.second;
				for (const auto& pt_rel : tier)
				{
					auto pt_abs = glm::ivec2(p.x + pt_rel.x, p.y + pt_rel.y);
					if (!vismap.InRange(pt_abs))
						continue;

					auto pt_rel_abs = glm::abs(pt_rel);
					auto pt_sign = glm::sign(pt_rel);
					auto pt_prev = pt_rel - pt_sign;
					const float curvis_cmp = 0.05f;
					float curvis;
					if (((pt_rel.x == 0) | (pt_rel.y == 0)) || (pt_rel_abs.x == pt_rel_abs.y))
					{
						// propagate visibility fully, take into account blocked_prev, so propagate scaled visibility really
						curvis = calc_vis(vismap, p, pt_prev, mVisMap, mLoS);
					}
					else
					{
						// which dimension is the greatest?
						int max_mag_coord = pt_rel_abs.x > pt_rel_abs.y ? 0 : 1;

						// create the two points we will sample from
						glm::ivec2 nb_near = pt_prev;
						glm::ivec2 nb_far = pt_rel;
						nb_far[max_mag_coord] = pt_prev[max_mag_coord];

						// Use a custom distance function to generate percentages: for starters, manh distance. Alts: eucli, eucli^2, more TODO
						auto dnear = pgn::norm_inf(nb_near);
						auto dfar = pgn::norm_inf(nb_far);
						auto ddenom = 1.0f / float(dnear + dfar);
						float pc_near = dfar*ddenom;
						float pc_far = dnear*ddenom;

						auto mhdnear = pgn::norm_1(nb_near);
						auto mhdfar = pgn::norm_1(nb_far);
						auto mhddenom = 1.0f / float(mhdnear + mhdfar);
						float mhpc_near = mhdfar*mhddenom;
						float mhpc_far = mhdnear*mhddenom;

						// now that we have the percentages, get the visibility of those points multiplied by the blockage status and propagate
						auto vis_near = calc_vis(vismap, p, nb_near, mVisMap, mLoS);
						auto vis_far = calc_vis(vismap, p, nb_far, mVisMap, mLoS);
						auto bnear = mBlockMap(glm::ivec2(nb_near.x + mLoS, nb_near.y + mLoS));
						auto bfar = mBlockMap(glm::ivec2(nb_far.x + mLoS, nb_far.y + mLoS));
						bool bthis = vismap(p + pt_rel);

						float f0 = 0.5f, f1 = 0.5f;

						curvis = (bnear && bfar) ? 0.0f : vis_near*pc_near + vis_far*pc_far;
						//curvis = (bnear && bfar) ? 0.0f : vis_near*f0 + vis_far*f1;
						mBlockMap(glm::ivec2(pt_rel.x + mLoS, pt_rel.y + mLoS))= bnear || bfar || bthis || (curvis < curvis_cmp);
					}
					mVisMap(pt_rel.x + mLoS, pt_rel.y + mLoS) = curvis;
					vis(p + pt_rel) = curvis;
					if (curvis > curvis_cmp)
						lospts.push_back(glm::ivec2(p.x + pt_rel.x, p.y + pt_rel.y));
				}
			}

			//PrintVis(vis, dmap, p);
		}

	}
}