#pragma once

#include <vector>

#include <glm/glm.hpp>

namespace pgn
{
	//! 2D array data. TODO: Sub-array if needed
	template<class T>
	struct cArray2D
	{
	public:
		virtual ~cArray2D(){}
		void Resize(size_t zW, size_t zH)								{mWidth=zW;mHeight=zH;mData.resize(mWidth*mHeight);}
		size_t Width() const													{return mWidth;}
		size_t Height() const													{return mHeight;}
		const T& operator()(const size_t zX, const size_t zY) const		{return mData.at(LinearIdx(zY,zX));}
		T& operator()(const size_t zX, const size_t zY)					{return mData.at(LinearIdx(zY,zX));}
		const T * Raw() const												{return &mData.first();}
		void SetRaw(T * ptr, size_t zW, size_t zH)					{Resize(zW,zH);memcpy(&mData.front(),ptr,mData.size()*sizeof(T));}

		bool InRange(const glm::ivec2& v) const						{ return InRange(v.x, v.y); }
		bool InRange(int x, int y) const							{ return (x >= 0) && (x < int(mWidth)) && (y >= 0) && (y < int(mHeight)); }

		//! Alternative accessors
		const T& operator()(const glm::ivec2& zV) const		{ return (*this)(zV.x, zV.y); }
		T& operator()(const glm::ivec2& zV)					{ return (*this)(zV.x, zV.y); }
		
	protected:
		size_t LinearIdx(const size_t zRow, const size_t zCol) const		{return zRow*mWidth + zCol;}
	protected:
		size_t		   mHeight;
		size_t		   mWidth;
		std::vector<T> mData;
	};

	//--------------------------------------------------------------
	template<class T, class Visitor>
	void VisitArray(T& data, Visitor& v)
	{ 
		for (auto i = 0; i < data.Height();++i)
			for (auto j = 0; j < data.Width(); ++i)
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