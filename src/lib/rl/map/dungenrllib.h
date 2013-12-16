#pragma once

#include "dungen.h"
#include <rl/map/gen/map.h>
#include <rl/map/gen/mapgenerators.h>

namespace pgn
{
	class cDunGenRLLib : public cDunGen
	{
	public:
		virtual ~cDunGenRLLib(){}
		void Generate(array_type& zMap, size_t w, size_t h);
	protected:
		virtual void GenerateRLLib(RL::CMap& zMap) {}
	private:
		RL::CMap mMap;
	};
	
	//! Ant nest
	class cDunGenRLLib_AntNest : public cDunGenRLLib
	{
	public:
		cDunGenRLLib_AntNest(bool withRooms = false):mWithRooms(withRooms){}
		virtual void GenerateRLLib(RL::CMap& zMap) {RL::CreateAntNest(zMap, mWithRooms);}
	private:
		bool mWithRooms;
	};

	//! Caves
	class cDunGenRLLib_Caves : public cDunGenRLLib
	{
	public:
		cDunGenRLLib_Caves(int iterations = 1, float density=0.65f) :mIterations(iterations), mDensity(density){}
		virtual void GenerateRLLib(RL::CMap& zMap) { RL::CreateCaves(zMap, mIterations, mDensity); }
	private:
		int mIterations;
		float mDensity;
	};

	//! Maze
	class cDunGenRLLib_Maze : public cDunGenRLLib
	{
	public:
		cDunGenRLLib_Maze(bool allow_loops = false) :mAllowLoops(allow_loops){}
		virtual void GenerateRLLib(RL::CMap& zMap) { RL::CreateMaze(zMap, mAllowLoops); }
	private:
		bool mAllowLoops;
	};

	//! Mines
	class cDunGenRLLib_Mines : public cDunGenRLLib
	{
	public:
		cDunGenRLLib_Mines(int max_rooms = 10) :mMaxRooms(max_rooms){}
		virtual void GenerateRLLib(RL::CMap& zMap) { RL::CreateMines(zMap, mMaxRooms); }
	private:
		int mMaxRooms;
	};

	//! City
	class cDunGenRLLib_City : public cDunGenRLLib
	{
	public:
		cDunGenRLLib_City(int num_buildings = 5) :mNumBuildings(num_buildings){}
		virtual void GenerateRLLib(RL::CMap& zMap) { RL::CreateSimpleCity(zMap, mNumBuildings); }
	private:
		int mNumBuildings;
	};

	//! Space shuttle
	class cDunGenRLLib_SpaceShuttle : public cDunGenRLLib
	{
	public:
		cDunGenRLLib_SpaceShuttle(int num_rooms = 15, bool mirror_vertical = false, bool rooms_same = true) :mNumRooms(num_rooms), mMirrorVertical(mirror_vertical), mRoomsSame(rooms_same){}
		virtual void GenerateRLLib(RL::CMap& zMap) { RL::CreateSpaceShuttle(zMap, mNumRooms,mMirrorVertical,mRoomsSame ); }
	private:
		int mNumRooms;
		bool mMirrorVertical;
		bool mRoomsSame;
	};

	//! Std dungeon
	class cDunGenRLLib_StdDungeon : public cDunGenRLLib
	{
	public:
		cDunGenRLLib_StdDungeon(int num_rooms = 10, bool has_doors = true) :mNumRooms(num_rooms), mHasDoors(has_doors){}
		virtual void GenerateRLLib(RL::CMap& zMap) { RL::CreateStandardDungeon(zMap, mNumRooms, mHasDoors); }
	private:
		int mNumRooms;
		bool mHasDoors;
	};


}