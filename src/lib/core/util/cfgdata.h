#pragma once

#include <vector>
#include <string>
#include <map>

#include <core/util/conversions.h>

/*
	TODO: description, get/set single uses get/set vectors
*/

namespace pgn
{
	struct cCfgData
	{
	public:
		typedef std::map< std::string, std::vector<std::string> > map_type;
	
	public:
		void Merge(const cCfgData& zOtherData);
		template<class T>
		bool Get(const std::string& zKey, std::vector<T>& zValues) const;
		template<class T>
		bool Set(const std::string& zKey, const std::vector<T>& zValues);
		template<class T>
		bool Get(const std::string& zKey, T& zValue) const;
		template<class T>
		bool Set(const std::string& zKey, const T& zValue);

	public:
		map_type mData;
	};

	//----------------------------------------------------
	template<class T>
	bool cCfgData::Get(const std::string& zKey, std::vector<T>& zValues) const
	{
		auto itr = mData.find(zKey);
		if(itr != mData.end())
		{
			auto& vals = itr->second;
			zValues.clear();
			zValues.reserve( vals.size());
			bool success = true;
			for( auto& item : vals)
			{
				T v;
				success &= pgn::from_string<T>(item, v);
				zValues.push_back(v);
			}
			return success;
		}
		else
			return false;
	}
	//----------------------------------------------------
	template<class T>
	bool cCfgData::Get(const std::string& zKey, T& zValue) const
	{
		auto itr = mData.find(zKey);
		if(itr != mData.end())
		{
			auto& vals = itr->second;
			if(vals.size() != 1)
				return false;
			else
			{
				return pgn::from_string<T>( vals[0], zValue);
			}
		}
		else
			return false;
	}
	//----------------------------------------------------
	template<class T>
	bool cCfgData::Set(const std::string& zKey, const std::vector<T>& zValues)
	{
		auto& vals = mData[zKey];
		vals.clear();
		vals.reserve(zValues.size());
		for( auto& item : zValues)
			vals.push_back(pgn::to_string(item));
		return true;
	}
	//----------------------------------------------------
	template<class T>
	bool cCfgData::Set(const std::string& zKey, const T& zValue)
	{
		auto& vals = mData[zKey];
		vals.resize(1);
		vals.front() = pgn::to_string(zValue);
		return true;
	}
}
