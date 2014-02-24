#pragma once

#include <deque>
#include <memory>
#include <vector>

#include <core/container/Array2D.h>
#include <rlut/path/Astar.h>
#include "Constraints.h"

namespace pgn
{
	namespace rlut
	{
		struct cDiagDistMatrix
		{
			typedef ::std::vector<size_t> type;
			void Resize(size_t elem_num, size_t value);
			size_t& operator()(size_t x, size_t y);
			void update_set(size_t a0, size_t a1, size_t corradd = 0);
			void update_single(size_t c, size_t a1);

			type mData;
			size_t mDataElems;
		};


		struct eMapData
		{
			static const int unused = 0;
			static const int conn = 1;
			static const int room = 2;
			static const int corridor = 4;
			static const int perimeter = 8;
			static const int blocked = 16;
			static const int entry = 32;
			static const int exit = 64;
		};

		struct cArea;
		typedef ::std::shared_ptr<cArea> cAreaPtr;
		struct cRoom;

		struct cSharedPerimeterInfo
		{
			cSharedPerimeterInfo() :mIsConnection(false){}

			size_t	mSrcArea;
			bool	mIsConnection;
		};

		struct cWorkspace
		{
			static const size_t inf = 0xFFFFFFFF / 2;
			typedef ::std::vector<size_t> cIsle;

			cWorkspace();
			void Init(int w, int h);
			void Print();

			cDiagDistMatrix								mAreaConnDistanceMap;
			cArray2D<::std::vector<size_t>>			mPerimeterMap;
			cArray2D<::std::vector<size_t>>			mConnectionMap;
			::std::vector<cAreaPtr>						mAreas;
			cArray2D<int>							mMapData;
			cAstar									mAstar;
			::std::deque<glm::ivec2>						mAstarPath;
			cConstraints								mConstraints;
			cArray2D<size_t>						mAreaDistances;
			::std::vector<cIsle>							mIslands;
			::std::vector<bool>							mAreasVisited;

			size_t randu(const glm::ivec2& v);


			float MoveCost(const glm::ivec2& p, int validBits, int invBits) const;

			void clear_perimeter(const cArea& area);
			bool is_perimeter_corner(const glm::ivec2& v) const;
			void generate_dungeon(size_t numRooms);

			void calc_area_to_area_distances();
			void calc_islands();
			void add_to_isle(const cAreaPtr&, cIsle& isle);
			void connect_islands();
			bool connect_island(cIsle& i0);

			void connect_islands2();
			void calc_islands2();

			void add_feature_entry_exit();
		};
	}
}