#include "cfgdata.h"

namespace pgn
{
	void cCfgData::Merge(const cCfgData& zOtherData)
	{
		mData.insert(zOtherData.mData.begin(), zOtherData.mData.end());
	}
}