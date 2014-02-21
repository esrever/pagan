#include "fov1.h"

#include <core/math/norm.h>
#include <ai/utils/shape/ShapeCalc.h>

namespace pgn
{
	namespace ai
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
		void cFov1::Init(size_t los)
		{
			mLoS = los;
		}

		static void ConstantDir(glm::ivec2 start, 
								const glm::ivec2& end, 
								const cArray2D<bool>& vismap, 
								std::vector<glm::ivec2>& lospts, 
								cArray2D<float>& vis)
		{
			glm::ivec2 p_abs_cur = start;
			glm::ivec2 p_abs_prev = start;

			glm::ivec2 vd = end - start;
			glm::ivec2 vd_abs = glm::abs(end - start);
			int d = glm::max(vd_abs.x, vd_abs.y);
			
			glm::ivec2 incr = glm::sign(vd);

			for (int i = 1; i < d; ++i)
			{
				p_abs_prev = p_abs_cur;
				p_abs_cur += incr;

				if (!vismap(p_abs_prev))
					break;
				
				vis(p_abs_cur) = 1.0f;
				lospts.push_back(p_abs_cur);
			}
		}

		void cFov1::Calc(const glm::ivec2& p, const cArray2D<bool>& vismap, std::vector<glm::ivec2>& lospts, cArray2D<float>& vis)
		{
			auto iters = pgn::ai::cShapeCalc<pgn::ai::cDiskDistance>::Get(mLoS, mLoS);
			for (auto it = iters.first; it != iters.second; ++it)
			{
				lospts.clear();
				lospts.push_back(p);
				vis(p) = 1.0f;

				const auto& tgt_rel = *it;
				
				if ((tgt_rel.x == 0) || (tgt_rel.y == 0) ||  (glm::abs(tgt_rel.x) == glm::abs(tgt_rel.y)))
				{
					ConstantDir(p, p + tgt_rel, vismap, lospts, vis);
				}
				else
				{
					float slope = tgt_rel.y / float(tgt_rel.x);
					
					float curvis = 1.0f;
					// y-major
					if (glm::abs(tgt_rel.y) > glm::abs(tgt_rel.x))
					{
						for (int i = 1; i < glm::abs(tgt_rel.y); ++i)
						{
							int cury = i*glm::sign(tgt_rel.y);
							float x = cury / slope;
							int x0 = int(x);
							float t = fabs(x - x0);
							int x1 = x0 + (x < 0 ? -1 : 1);

							auto p0 = glm::ivec2(x0, cury) + p;
							auto p1 = glm::ivec2(x1, cury) + p;

							float v0 = 0.0f;
							float v1 = 0.0f;
							if (vis.InRange(p0))
							{
								if (t <= 0.5)
								vis(p0) = std::max(vis(p0), curvis);
								v0 = vismap(p0) ? 1.0f : 0.0f;
							}
							
							if (vis.InRange(p1))
							{
								if (t >= 0.5)
								vis(p1) = std::max(vis(p1), curvis);
								v1 = vismap(p1) ? 1.0f : 0.0f;
							}

							curvis = (1 - t)*v0 + t*v1;
						}
					}
					else
					{
						for (int i = 1; i < glm::abs(tgt_rel.x); ++i)
						{
							int curx = i*glm::sign(tgt_rel.x);
							float y = slope * curx;
							int y0 = int(y);
							float t = fabs(y - y0);
							int y1 = y0 + (y < 0 ? -1 : 1);

							auto p0 = glm::ivec2(curx, y0) + p;
							auto p1 = glm::ivec2(curx, y1) + p;
							
							float v0 = 0.0f;
							float v1 = 0.0f;
							if (vis.InRange(p0))
							{
								if (t <= 0.5)
								vis(p0) = std::max(vis(p0), curvis);
								v0 = vismap(p0) ? 1.0f : 0.0f;
							}

							if (vis.InRange(p1))
							{
								if (t >= 0.5)
								vis(p1) = std::max(vis(p1), curvis);
								v1 = vismap(p1) ? 1.0f : 0.0f;
							}


							curvis = (1 - t)*v0 + t*v1;
						}
					}
				}
			}
		}

	}
}