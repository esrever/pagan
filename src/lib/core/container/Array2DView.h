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
		typedef std::function<void(size_t, size_t, typename T::const_reference)> visit_read_ext_func;
		typedef std::function<void(size_t, size_t, typename T::reference)> visit_write_ext_func;
		typedef std::function<void(typename T::const_reference)> visit_read_func;
		typedef std::function<void(typename T::reference)> visit_write_func;

		// ctor
		cArrayView(T& ar, const cArrayShape2D& orgShape, size_t w=0, size_t h=0, size_t x = 0, size_t y = 0) : mStorageRef(&ar), mOrgShape(orgShape),mShape(w, h, x, y) {};
		cArrayView(T& ar, size_t w = 0, size_t h = 0, size_t x = 0, size_t y = 0) : mStorageRef(&ar), mShape(w, h, x, y), mOrgShape(mShape) {};
		cArrayView& operator = (const cArrayView& v) {
			mStorageRef = v.mStorageRef;  mShape = v.mShape; mOrgShape = v.mOrgShape;
		    return *this; }

		// Linear index
		size_t LinearIdx(size_t x, size_t y) const { return mShape.RealX(x) + mOrgShape.Width()*mShape.RealY(y); }

		// op-access
		typename T::reference       operator()(size_t x, size_t y)       { return mStorageRef->Get(LinearIdx(x, y)); }
		typename T::const_reference operator()(size_t x, size_t y) const  { return mStorageRef->Get(LinearIdx(x, y)); }

		// visitor
		void VisitR(visit_read_func f) const;
		void VisitW(visit_write_func f);
		void VisitRext(visit_read_ext_func f) const;
		void VisitWext(visit_write_ext_func f);

		void Fill(const typename T::data_type& v) { VisitW([&v](typename T::data_type& vout){vout = v; }); }

		// data-access
		const cArrayShape2D& Shape() const { return mShape; }
		const cArrayShape2D& OrgShape() const { return *mOrgShape; }
		const T& Storage() const { return *mStorageRef; }

	private:
		cArrayShape2D     mShape;
	    cArrayShape2D	  mOrgShape;
		T * mStorageRef;
	};

	template<class T>
	void cArrayView<T>::VisitR(typename cArrayView<T>::visit_read_func f) const
	{
		for (size_t y = 0; y < mShape.Height(); ++y)
		for (size_t x = 0; x < mShape.Width(); ++x)
			f((*this)(x,y));
	}

	template<class T>
	void cArrayView<T>::VisitW(typename cArrayView<T>::visit_write_func f)
	{
		for (size_t y = 0; y < mShape.Height(); ++y)
		for (size_t x = 0; x < mShape.Width(); ++x)
			f((*this)( x,y));
	}

	template<class T>
	void cArrayView<T>::VisitRext(typename cArrayView<T>::visit_read_ext_func f) const
	{
		for (size_t y = 0; y < mShape.Height(); ++y)
		for (size_t x = 0; x < mShape.Width(); ++x)
			f(x, y, (*this)(x, y));
	}

	template<class T>
	void cArrayView<T>::VisitWext(typename cArrayView<T>::visit_write_ext_func f)
	{
		for (size_t y = 0; y < mShape.Height(); ++y)
		for (size_t x = 0; x < mShape.Width(); ++x)
			f(x, y,(*this)(x,y));
	}

}