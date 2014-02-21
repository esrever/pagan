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


		static void CastLight( int row, float start, float end, int xx, int xy, int yx, int yy,
							   const glm::ivec2& p, const cArray2D<bool>& vismap, std::vector<glm::ivec2>& lospts, cArray2D<float>& vis, size_t los)
		{
			float newStart = 0.0f;
			if (start < end)
				return;
			
			bool blocked = false;
			for (int distance = row; distance <= int(los) && !blocked; distance++) {
				int deltaY = -distance;
				for (int deltaX = -distance; deltaX <= 0; deltaX++) {
					int currentX = p.x + deltaX * xx + deltaY * xy;
					int currentY = p.y + deltaX * yx + deltaY * yy;
					float leftSlope = (deltaX - 0.5f) / (deltaY + 0.5f);
					float rightSlope = (deltaX + 0.5f) / (deltaY - 0.5f);

					if (!(currentX >= 0 && currentY >= 0 && currentX < int(vismap.Width()) && currentY < int(vismap.Height())) || start < rightSlope) {
						continue;
					}
					else if (end > leftSlope) {
						break;
					}

					//check if it's within the lightable area and light if needed
					auto d = pgn::norm_2(glm::ivec2(deltaX, deltaY));
					if (d <= los) {
						float bright = (float)(1 - (d / los));
						vis( glm::ivec2(currentX,currentY)) = bright;
					}

					if (blocked) { //previous cell was a blocking one
						if (vismap(currentX,currentY) == 0.0f ) {//hit a wall
							newStart = rightSlope;
							continue;
						}
						else {
							blocked = false;
							start = newStart;
						}
					}
					else {
						if (vismap(currentX, currentY) == 0.0f && distance < int(los)) {//hit a wall within sight line
							blocked = true;
							CastLight(distance + 1, start, leftSlope, xx, xy, yx, yy, p, vismap, lospts, vis, los);
							newStart = rightSlope;
						}
					}
				}
			}
		}

		void cFov1::Calc(const glm::ivec2& p, const cArray2D<bool>& vismap, std::vector<glm::ivec2>& lospts, cArray2D<float>& vis)
		{
			vis(p) = 1.0f;
			lospts.clear();
			lospts.push_back(p);

			glm::ivec2 dirs[4] = { glm::ivec2(1, 1), glm::ivec2(1, -1), glm::ivec2(-1, 1), glm::ivec2(-1, -1) };
			for (size_t i = 0; i < 4; ++i)
			{
				CastLight(1, 1.0f, 0.0f, 0, dirs[i].x, dirs[i].y, 0, p, vismap, lospts, vis,mLoS);
				CastLight(1, 1.0f, 0.0f, dirs[i].x, 0, 0, dirs[i].y, p, vismap, lospts, vis, mLoS);
			}
		}

	}
}