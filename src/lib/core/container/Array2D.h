#pragma once

#include <vector>

namespace pgn
{
	//! 2D array data. TODO: Sub-array if needed
	template<class T>
	struct cArray2D
	{
	public:

		void Resize(size_t zRows, size_t zCols)								{mRows=zRows;mCols=zCols;mData.resize(mRows*mCols);}
		size_t Rows() const													{return mRows;}
		size_t Cols() const													{return mCols;}
		const T& operator()(const size_t zRow, const size_t zCol) const		{return mData.at(LinearIdx(zRow,zCol));}
		T& operator()(const size_t zRow, const size_t zCol)					{return mData.at(LinearIdx(zRow,zCol));}
		const T * Raw() const												{return &mData.first();}
		void SetRaw(T * ptr, size_t zRows, size_t zCols)					{Resize(zRows,zCols);memcpy(&mData.front(),ptr,mData.size()*sizeof(T));}
		
	private:
		size_t LinearIdx(const size_t zRow, const size_t zCol) const		{return zRow*mCols + zCol;}
	private:
		size_t		   mRows;
		size_t		   mcols;
		std::vector<T> mData;
	};
}