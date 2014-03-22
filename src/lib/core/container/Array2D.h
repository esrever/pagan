#pragma once

#include "ArrayStorage.h"
#include "Array2DView.h"

#include <glm/glm.hpp>

namespace pgn
{

	template<class T, class S = cDenseStorage<T>>
	struct cArray2D
	{
		// typedefs 
		typedef S storage_type;
		typedef T data_type;
		typedef cArrayView< S > view_type;

		// ctor
		cArray2D(size_t w = 0, size_t h = 0, const T& v = T()) :mStorage(), mView(mStorage, w, h){ Resize(w, h, v); }

		// functions
		const view_type& View() const { return mView; }
		view_type& View() { return mView; }
		view_type CreateView(size_t w, size_t h, size_t x, size_t y) { return view_type(mStorage, mView.Shape(), w, h, x, y); }
		const view_type CreateView(size_t w, size_t h, size_t x, size_t y) const { return view_type(mStorage, mView.Shape(), w, h, x, y); }
		void Resize(size_t w, size_t h, const T& v = T()) { mStorage.Resize(w* h, v); mView = view_type(mStorage, w, h); }

		// helpers - access shape
		size_t Width() const { return mView.Shape().Width(); }
		size_t Height() const { return mView.Shape().Height(); }

		// helpers - access view
		typename S::reference operator()(size_t x, size_t y)       { return mStorage.Get(mView.LinearIdx(x, y)); }
		typename S::const_reference operator()(size_t x, size_t y) const  { return mStorage.Get(mView.LinearIdx(x, y)); }
		bool InRange(size_t x, size_t y) const { return mView.Shape().InRange(x, y); }

		typename S::const_reference operator()(const glm::ivec2& v) const { return (*this)(v.x, v.y); }
		typename S::reference operator()(const glm::ivec2& v)             { return (*this)(v.x, v.y); }
		bool InRange(const glm::ivec2& v) const        { return InRange(v.x, v.y); }

		// helpers - fill
		void Fill(const T& val) { mStorage.Fill(val); }

	private:
		storage_type    mStorage;
		view_type       mView;
	};

}