#include "dungenrllib.h"

#include <rl/components/WorldTraits.h>

namespace pgn
{
	void cDunGenRLLib::Generate(array_type& zMap, size_t w, size_t h)
	{
		mMap.Resize(w, h);
		GenerateRLLib(mMap);
		zMap.Resize(w, h);

		for (unsigned i = 0; i < mMap.GetHeight();++i)
		{
			for (unsigned j = 0; j < mMap.GetWidth(); ++j)
			{
				zMap(j, i) = size_t(mMap.GetCell(j, i));
			}
		}

	}
}