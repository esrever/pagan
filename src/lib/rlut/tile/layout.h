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
				typedef cArray2D<bool> bool_map_type;
				typedef cArray2D<float> movecost_map_type;
				typedef cArray2D<cECS::cArchetypeWithDataConst> dense_archetypes_type;
				typedef cArray2D<cECS::cEntityWithDataConst> dense_entities_type;
				typedef cArray2D<cECS::cEntityWithDataConst, cSparseStorage<cECS::cEntityWithDataConst> > sparse_entities_type;
				typedef std::map<cECS::cEntityWithDataConst, glm::ivec2> sparse_map_type;

				void Init(const rlut::cWorkspace& ws, const std::map<std::string, std::string>& tiles);

				const dense_archetypes_type& BgEntities() const { return mBgEntities; }
				const sparse_entities_type& FgEntities() const { return mFgEntities; }
				const sparse_map_type& Actors() const { return mActors; }

				dense_archetypes_type& BgEntities() { return mBgEntities; }
				sparse_entities_type& FgEntities() { return mFgEntities; }
				sparse_map_type& Actors() { return mActors; }

				bool_map_type& Explored(size_t id) { return mExplored[id]; }
				bool_map_type& Visible(size_t id)  { return mVisible[id]; }

				void SetActor(cECS::cEntityWithDataConst ed);
				void RemoveActor(cECS::cEntityWithDataConst ed);
				
				const movecost_map_type StaticMoveCosts() const { return mStaticMoveCosts; }
				const movecost_map_type MoveCosts() const { return mMoveCosts; }

			private:
				
				dense_archetypes_type	mBgEntities;
				sparse_entities_type	mFgEntities;
				sparse_map_type			mActors;

				//! move costs of bg/fg entities that don't change frequently. Useful for longer path planning, without having to care about monsters blocking the way
				//! A* update is triggers if the static move cost map changes
				movecost_map_type		mStaticMoveCosts;

				//! move costs after actors, these change frequently
				movecost_map_type		mMoveCosts;

				//! Obstacle mask, for visibility
				bool_map_type			mObstacle;

				//! Explored and visible masks, by entity
				std::map< size_t, bool_map_type> mExplored;
				std::map< size_t, bool_map_type> mVisible;
		};

		// Related utilities
		glm::ivec2 GetCenteredView(const glm::ivec2& orgDims,
		  						   const glm::ivec2& center,
								   const glm::ivec2& tgtSize);

	}
	DECL_SERIALIZE_INTERFACE(rl::cLayout);

}

