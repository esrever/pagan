#pragma once

#include <vector>
#include <string>
#include <map>

#include <core/util/conversions.h>

namespace pgn
{
	/*
		Configuration data accessor : map< string, vector<string> >
	*/
	struct cCfgData
	{
	public:
        typedef std::string map_key_type;
        typedef std::vector<std::string> map_value_type;
		typedef std::map< map_key_type, map_value_type > map_type;
	
	public:
		void Merge(const cCfgData& zOtherData);
		const map_type& Data() const	{return mData;}
		
		template<class T>
		bool Get(const std::string& zKey, std::vector<T>& zValues) const;
		template<class T>
		bool Set(const std::string& zKey, const std::vector<T>& zValues);
		template<class T>
		bool Get(const std::string& zKey, T& zValue) const;
		template<class T>
		bool Set(const std::string& zKey, const T& zValue);

	private:
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
