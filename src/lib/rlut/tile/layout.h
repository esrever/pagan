#pragma once

#include <core/container/Array2D.h>
#include <core/serialize/serialize.h>

#include <ecs/ecs.h>

namespace pgn
{
	namespace rlut
	{
		struct cWorkspace;
	}

	namespace rl
	{
		class cLayout
		{
			public:
				typedef cArray2D<cECS::cArchetypeWithDataConst> dense_archetypes_type;
				typedef cArray2D<cECS::cEntityWithDataConst> dense_entities_type;
				typedef cArray2D<cECS::cEntityWithDataConst, cSparseStorage<cECS::cEntityWithDataConst> > sparse_entities_type;

				void Init(const rlut::cWorkspace& ws, const std::map<std::string, std::string>& tiles);

				const dense_archetypes_type& BgEntities() const { return mBgEntities; }
				const sparse_entities_type& FgEntities() const { return mFgEntities; }

			private:
				
				dense_archetypes_type	mBgEntities;
				sparse_entities_type	mFgEntities;
		};
	}
	DECL_SERIALIZE_INTERFACE(rl::cLayout);

}