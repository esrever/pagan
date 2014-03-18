#include "fov_rsc.h"

#include <core/math/norm.h>
#include <rlut/utils/shape/ShapeCalc.h>

namespace pgn
{
	namespace rlut
	{
		/*
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
		*/

		//-----------------------------------------------------------------------------------
		void cFovRsc::Init(size_t los)
		{
			mLoS = los;
		}

		static void CastLight( int row, float start, float end, int xx, int xy, int yx, int yy,
			const glm::ivec2& p, const cArray2D<bool>& obstacles, on_visible_func onvis, size_t los)
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

					if (!(currentX >= 0 && currentY >= 0 && currentX < int(obstacles.Width()) && currentY < int(obstacles.Height())) || start < rightSlope) {
						continue;
					}
					else if (end > leftSlope) {
						break;
					}

					//check if it's within the lightable area and light if needed
					auto d = pgn::norm_2(glm::ivec2(deltaX, deltaY));
					if (d <= los) {
						float bright = (float)(1 - (d / los));
						onvis(glm::ivec2(currentX, currentY), bright);
					}

					if (blocked) { //previous cell was a blocking one
						if (obstacles(currentX, currentY)) {//hit a wall
							newStart = rightSlope;
							continue;
						}
						else {
							blocked = false;
							start = newStart;
						}
					}
					else {
						if (obstacles(currentX, currentY) && distance < int(los)) {//hit a wall within sight line
							blocked = true;
							CastLight(distance + 1, start, leftSlope, xx, xy, yx, yy, p, obstacles, onvis, los);
							newStart = rightSlope;
						}
					}
				}
			}
		}

		void cFovRsc::Calc(const glm::ivec2& p, const cArray2D<bool>& obstacles, on_visible_func onvis)
		{
			onvis(p, 1.0f);

			glm::ivec2 dirs[4] = { glm::ivec2(1, 1), glm::ivec2(1, -1), glm::ivec2(-1, 1), glm::ivec2(-1, -1) };
			for (size_t i = 0; i < 4; ++i)
			{
				CastLight(1, 1.0f, 0.0f, 0, dirs[i].x, dirs[i].y, 0, p, obstacles, onvis, mLoS);
				CastLight(1, 1.0f, 0.0f, dirs[i].x, 0, 0, dirs[i].y, p, obstacles, onvis, mLoS);
			}
		}

	}
}