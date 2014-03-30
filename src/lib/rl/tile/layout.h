#pragma once

#include <core/util/color.h>
#include <core/container/Array2D.h>
#include <core/serialize/serialize.h>

#include <ecs/ecs.h>
#include <rl/tile/tilestore.h>

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
				typedef std::tuple< ecs::cArchetypeWithData /*arch*/,
								    ecs::cEntityWithData /*arch-inst*/, 
									std::string /*tile-type*/,
									std::string /*bg-tile*/,
									std::string /*tag*/> legend_entry_t;
				typedef cArray2D<bool> bool_map_type;
				typedef cArray2D<float> movecost_map_type;

				void Init(const cArray2D<SDL_Color>& ws, const std::map<SDL_Color, legend_entry_t>& legend, std::map<std::string, ecs::cEntityWithData>& bgtiles);

				const glm::uvec2& Dims() const { return mDims; }
				cTileStoreSparse1& Actors() { return mActors; }
				const cTileStoreSparse1& Actors() const { return mActors; }
				cTileStoreDense1& Bg() { return mBg; }
				const cTileStoreDense1& Bg() const { return mBg; }
				cTileStoreSparse1& Fg() { return mFg; }
				const cTileStoreSparse1& Fg() const { return mFg; }
				
				const movecost_map_type& StaticMoveCosts() const { return mStaticMoveCosts; }
				const movecost_map_type& MoveCosts() const { return mMoveCosts; }
				const bool_map_type& Obstacles() const { return mObstacles; }
				const bool_map_type& StaticObstacles() const { return mStaticObstacles; }

				const glm::ivec2& Entry() const { return mEntry; }
				const glm::ivec2& Exit() const { return mExit; }

			public:
				glm::uvec2				mDims;
				glm::ivec2				mEntry, mExit;

				//! move costs of bg/fg entities that don't change frequently. Useful for longer path planning, without having to care about monsters blocking the way
				//! A* update is triggers if the static move cost map changes
				movecost_map_type		mStaticMoveCosts;
				//! move costs after actors, these change frequently
				movecost_map_type		mMoveCosts;
				//! Obstacle mask, for visibility
				bool_map_type			mObstacles;
				bool_map_type			mStaticObstacles;
				
				cTileStoreDense1		mBg;
				cTileStoreSparse1		mFg;
				cTileStoreSparse1		mActors;
		};

		// Related utilities
		glm::ivec2 GetCenteredView(const glm::ivec2& orgDims,
		  						   const glm::ivec2& center,
								   const glm::ivec2& tgtSize);

	}
	DECL_SERIALIZE_INTERFACE(rl::cLayout);

}

