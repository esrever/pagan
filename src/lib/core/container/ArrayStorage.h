#pragma once

#include <vector>
#include <map>

namespace pgn
{

	//! Storage classes provide means to fill with data, get and set values using 1d index, and get the raw (and default?) data
	template<class T>
	struct cDenseStorage
	{
		// typedefs
		typedef T data_type;
		typedef std::vector<T> container_type;
		typedef typename container_type::reference reference;
		typedef typename container_type::const_reference const_reference;

		// funcs
		void Resize(size_t n, const T& val = T()) { mData.resize(n, val); }
		void Fill(const T& val)        { mData.assign(mData.size(), val); }

		// access
		reference Get(size_t idx)             { return mData.at(idx); }
		const_reference Get(size_t idx) const { return mData.at(idx); }

		//! type-specific
		const container_type& Raw() const { return mData; }

	private:
		container_type mData;
	};

	template<class T>
	struct cSparseStorage
	{
		// typedefs
		typedef T data_type;
		typedef std::map<size_t, T> container_type;
		typedef T& reference;
		typedef const T& const_reference;

		// funcs
		void Resize(size_t n, const T& val = T()) { mData.clear(); mDefault = val; }
		void Fill(const T& val)        { mData.clear(); mDefault = val; }

		// access
		const_reference Get(size_t idx) const { auto it = mData.find(idx); return it == mData.end() ? mDefault : *it; }
		reference Get(size_t idx) { auto it = mData.find(idx); return it == mData.end() ? mDefault : *it; }

		//! type-specific
		const container_type& Raw() const { return mData; }
		const T& GetDefault() const { return mDefault; }

	private:
		container_type mData;
		data_type mDefault;
	};
}