#pragma once

#include <core/container/Array2D.h>
#include <core/serialize/serialize.h>

#include <ecs/ecs.h>

namespace pgn
{
	namespace ecs
	{
		namespace cmp
		{
			struct cLocation;
		}
	}
	namespace rl
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
				typedef cArray2D<ecs::cArchetypeWithDataConst> dense_archetypes_type;
				typedef cArray2D<ecs::cEntityWithDataConst> dense_entities_type;
				typedef cArray2D<ecs::cEntityWithDataConst, cSparseStorage<ecs::cEntityWithDataConst> > sparse_entities_type;
				typedef std::map<ecs::cEntityWithDataConst, glm::ivec2> sparse_map_type;

				void Init(const rl::cWorkspace& ws, const std::map<std::string, std::string>& tiles);

				const dense_archetypes_type& BgEntities() const { return mBgEntities; }
				const sparse_entities_type& FgEntities() const { return mFgEntities; }
				const sparse_map_type& Actors() const { return mActors; }

				dense_archetypes_type& BgEntities() { return mBgEntities; }
				sparse_entities_type& FgEntities() { return mFgEntities; }
				sparse_map_type& Actors() { return mActors; }

				void SetActor(ecs::cEntityWithDataConst ed);
				void RemoveActor(ecs::cEntityWithDataConst ed);
				
				const movecost_map_type& StaticMoveCosts() const { return mStaticMoveCosts; }
				const movecost_map_type& MoveCosts() const { return mMoveCosts; }
				const bool_map_type& Obstacles() const { return mObstacles; }

				const glm::ivec2& Entry() const { return mEntry; }
				const glm::ivec2& Exit() const { return mExit; }

				
				void UpdateLayout(ecs::cEntityWithDataConst ed, ecs::cmp::cLocation * zLocOld, ecs::cmp::cLocation * zLocNew);

			private:
				void AddTile(ecs::cEntityWithDataConst ed, const ecs::cmp::cLocation& zLocNew);
				void RemoveTile(ecs::cEntityWithDataConst ed, const ecs::cmp::cLocation& zLocOld);
				void UpdateTile(ecs::cEntityWithDataConst ed, const ecs::cmp::cLocation& zLocOld, const ecs::cmp::cLocation& zLocNew);


				void UpdateStaticMoveCosts();
				void UpdateMoveCosts();
				void UpdateObstacles();

			private:

				glm::ivec2				mEntry, mExit;
				
				dense_archetypes_type	mBgEntities;
				sparse_entities_type	mFgEntities;
				sparse_map_type			mActors;

				//! move costs of bg/fg entities that don't change frequently. Useful for longer path planning, without having to care about monsters blocking the way
				//! A* update is triggers if the static move cost map changes
				movecost_map_type		mStaticMoveCosts;

				//! move costs after actors, these change frequently
				movecost_map_type		mMoveCosts;

				//! Obstacle mask, for visibility
				bool_map_type			mObstacles;
		};

		// Related utilities
		glm::ivec2 GetCenteredView(const glm::ivec2& orgDims,
		  						   const glm::ivec2& center,
								   const glm::ivec2& tgtSize);

	}
	DECL_SERIALIZE_INTERFACE(rl::cLayout);

}

