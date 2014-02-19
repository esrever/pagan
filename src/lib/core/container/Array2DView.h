#pragma once

#include <functional>

#include "ArrayShape2D.h"
#include "ArrayStorage.h"

namespace pgn
{

	template<class T>
	struct cArrayView
	{
		// typedefs
		typedef std::function<void(size_t, size_t, const typename T::data_type&)> visit_read_func;
		typedef std::function<void(size_t, size_t, typename T::data_type&)> visit_write_func;

		// ctor
		cArrayView(T& ar, size_t w=0, size_t h=0, size_t x = 0, size_t y = 0) : mStorageRef(ar), mShape(w, h, x, y) {};
		cArrayView& operator = (const cArrayView& v) { mShape = v.mShape; mStorageRef = v.mStorageRef; return *this; }

		// op-access
		typename T::data_type&       operator()(size_t x, size_t y)       { return mStorageRef.Get(mShape.LinearIdx(x, y)); }
		const typename T::data_type& operator()(size_t x, size_t y) const  { return mStorageRef.Get(mShape.LinearIdx(x, y)); }

		// visitor
		void Visit(visit_read_func f) const;
		void Visit(visit_write_func f);

		// data-access
		const cArrayShape2D& Shape() const { return mShape; }
		const T& Storage() const { return mStorageRef; }

	private:
		cArrayShape2D     mShape;
		T& mStorageRef;
	};

	template<class T>
	void cArrayView<T>::Visit(typename cArrayView<T>::visit_read_func f) const
	{
		for (size_t y = 0; y < mShape.Height(); ++y)
		for (size_t x = 0; x < mShape.Width(); ++x)
			f(x, y, (*this)(mShape.RealX(x), mShape.RealY(y)));
	}

	template<class T>
	void cArrayView<T>::Visit(typename cArrayView<T>::visit_write_func f)
	{
		for (size_t y = 0; y < mShape.Height(); ++y)
		for (size_t x = 0; x < mShape.Width(); ++x)
			f(x, y, (*this)( mShape.RealX(x), mShape.RealY(y)));
	}

}