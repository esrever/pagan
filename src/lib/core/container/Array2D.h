#pragma once

#include <vector>

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

		//! Alternative accessors
		const T& operator()(const glm::ivec2& zV) const		{return (*this)(v.x,v.y);}
		T& operator()(const glm::ivec2& zV)					{return (*this)(v.x,v.y);}
		
	protected:
		size_t LinearIdx(const size_t zRow, const size_t zCol) const		{return zRow*mWidth + zCol;}
	protected:
		size_t		   mHeight;
		size_t		   mWidth;
		std::vector<T> mData;
	};
}