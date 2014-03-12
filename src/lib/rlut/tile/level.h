#pragma once

#include <core/container/Array2D.h>
#include <core/serialize/serialize.h>

#include <ecs/ecs.h>

namespace pgn
{
	namespace rl
	{
		class cLevel
		{
			public:
				typedef cArray2D<cECS::cEntityWithData> dense_entities_type;
				typedef cArray2D<cECS::cEntityWithData, cSparseStorage<cECS::cEntityWithData> > sparse_entities_type;

			private:

				dense_entities_type		mBgEntities;
				sparse_entities_type	mFgEntities;
		};

		DECL_SERIALIZE_INTERFACE(rl::cLevel);
	}
}