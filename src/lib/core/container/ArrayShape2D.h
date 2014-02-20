#pragma once

namespace pgn
{

	//! The shape does queries and conversions related to the shape
	struct cArrayShape2D
	{
		// ctor
		cArrayShape2D(size_t w = 0, size_t h = 0, size_t x = 0, size_t y = 0) :mW(w), mH(h), mX(x), mY(y){} // all zeros

		// funcs
		bool InRange(size_t x, size_t y) const { return InRange(x, 0, Width()) && InRange(y, 0, Height()); }

		// access
		size_t Width() const { return mW; }
		size_t Height() const { return mH; }
		size_t RealX(size_t x = 0) const { return mX + x; }
		size_t RealY(size_t y = 0) const { return mY + y; }

	private:
		static bool InRange(size_t v, size_t vmin, size_t vmax) { return (v >= vmin) && (v < vmax); }
	private:
		size_t mX, mY, mW, mH;
	};
}