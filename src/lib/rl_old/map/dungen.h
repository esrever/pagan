#pragma once

#include <core/container/Array2D.h>

namespace pgn
{
	enum eDungeonType
	{
		StdDungeon,
		Antnest,
		Caves,
		Maze,
		Mines,
		City,
		SpaceShuttle
	};

	class cDunGen
	{
		public:
			typedef cArray2D<size_t> array_type;
			virtual ~cDunGen(){}
			virtual void Generate(array_type& zMap, size_t w, size_t h){}
		private:
			
	};
}