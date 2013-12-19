#pragma once

#include <vector>
#include <map>

#include <glm/glm.hpp>

namespace pgn
{
	template<class T>
	struct cArrayDataDense
	{
		T& Get(size_t o) { return mData.at(o); }
		const T& Get(size_t o) const { return mData.at(o); }
		void Resize(size_t s) { mData.resize(s); }
		
		std::vector<T> mData;
	};

	template<class T>
	struct cArrayDataSparse
	{
		T& Get(size_t o) { assert(o < mTotal);  return mData[o]; }
		const T& Get(size_t o) const { assert(o < mTotal);  return mData[o]; }
		void Resize(size_t s) { mTotal = s; }

		std::map<size_t,T> mData;
		size_t mTotal;
	};

	//! 2D array data. TODO: Sub-array if needed
	template<typename T, typename D = cArrayDataDense<T> >
	struct cArray2D
	{
	public:
		typedef T value_type;
		typedef D storage_type;
		virtual ~cArray2D(){}
		void Resize(size_t zW, size_t zH)								{mWidth=zW;mHeight=zH;mData.Resize(mWidth*mHeight);}
		size_t Width() const													{return mWidth;}
		size_t Height() const													{return mHeight;}
		const T& operator()(const size_t zX, const size_t zY) const		{return mData.Get(LinearIdx(zY,zX));}
		T& operator()(const size_t zX, const size_t zY)					{return mData.Get(LinearIdx(zY,zX));}

		bool InRange(const glm::ivec2& v) const						{ return InRange(v.x, v.y); }
		bool InRange(int x, int y) const							{ return (x >= 0) && (x < int(mWidth)) && (y >= 0) && (y < int(mHeight)); }
		void Fill(int x, int y, int w, int h, const T& val);

		//! Alternative accessors
		const T& operator()(const glm::ivec2& zV) const		{ return (*this)(zV.x, zV.y); }
		T& operator()(const glm::ivec2& zV)					{ return (*this)(zV.x, zV.y); }

		const D& Store() const { return mData; }
		
	protected:
		size_t LinearIdx(const size_t zRow, const size_t zCol) const		{return zRow*mWidth + zCol;}
	protected:
		size_t		   mHeight;
		size_t		   mWidth;
		D			   mData;
	};

	//-------------------------------------------------------------
	template<class T,class D>
	void cArray2D<T,D>::Fill(int x, int y, int w, int h, const T& val)
	{
		int ye = y + h;
		for (int i = y; i < ye; ++i)
		{
			std::fill_n(mData.mData.begin() + LinearIdx(i, x), w, val);
		}		
	}

	//--------------------------------------------------------------
	template<class T, class Visitor>
	void VisitArray(T& data, Visitor& v)
	{ 
		for (auto i = 0; i < data.Height();++i)
			for (auto j = 0; j < data.Width(); ++j)
				v.visit(j, i, data(j, i));
	}
	
	/*
		template<class T>
		class cExampleVisitor
		{
			public:
				void visit(uint x, uint y, const T& data);  // const visit
				void visit(uint x, uint y, T& data);		// const visit
		}
	*/
}