#pragma once

#include <set>
#include <vector>
#include <functional>

#include <core/container/Array2D.h>
#include <core/container/ArrayStorage.h>
#include <ecs/ecs.h>

namespace pgn
{
	namespace rl
	{
		//! Entity storage in a level. Need to support a variety of queries and storage options.
		template<class SparseType , class DataType>
		class cTileStore
		{
			public:
				typedef DataType data_type;

				void Resize(size_t w, size_t h, const DataType& v = DataType()) { mCellEntities.Resize(w, h,v); }
				//! Move an entity to a new location pointed by its component
				void Move(const ecs::cEntityWithData& ed, const glm::ivec2& oldPos, const glm::ivec2& newPos);
				//! Add an entity
				void Add(const ecs::cEntityWithData& ed, const glm::ivec2& newPos);
				//! Remove an entity
				void Remove(const ecs::cEntityWithData& ed, const glm::ivec2& oldPos);
				//! Access the entities
				const std::set<ecs::cEntityWithData>& Entities() const { return mEntities; }
				//! Access the cells
				const cArray2D< DataType, SparseType>& Cells() const { return mCellEntities; }
			private:
				void TypedAdd(const ecs::cEntityWithData& ed, const glm::ivec2& newPos);
				void TypedRemove(const ecs::cEntityWithData& ed, const glm::ivec2& oldPos);
			private:
				cArray2D< DataType, SparseType> mCellEntities;
				std::set<ecs::cEntityWithData>  mEntities;
		};

		//---------------------------------------------------------------------------------
		typedef cTileStore< cDenseStorage< ecs::cEntityWithData>, ecs::cEntityWithData > cTileStoreDense1;
		typedef cTileStore< cDenseStorage< std::vector<ecs::cEntityWithData>>, std::vector<ecs::cEntityWithData> > cTileStoreDenseN;
		typedef cTileStore< cSparseStorage< ecs::cEntityWithData>, ecs::cEntityWithData > cTileStoreSparse1;
		typedef cTileStore< cSparseStorage< std::vector<ecs::cEntityWithData>>, std::vector<ecs::cEntityWithData> > cTileStoreSparseN;

		//---------------------------------------------------------------------------
		template<class SparseType, class DataType>
		void cTileStore<SparseType, DataType>::Move(const ecs::cEntityWithData& ed, const glm::ivec2& oldPos, const glm::ivec2& newPos)
		{
			TypedAdd(ed, newPos);
			TypedRemove(ed, oldPos);
		}
		//---------------------------------------------------------------------------
		template<class SparseType, class DataType>
		void cTileStore<SparseType, DataType>::Add(const ecs::cEntityWithData& ed, const glm::ivec2& newPos)
		{
			mEntities.insert(ed);
			TypedAdd(ed, newPos);
		}
		//---------------------------------------------------------------------------
		template<class SparseType, class DataType>
		void cTileStore<SparseType, DataType>::Remove(const ecs::cEntityWithData& ed, const glm::ivec2& oldPos)
		{
			mEntities.erase(ed);
			TypedRemove(ed,oldPos);
		}

		//---------------------------------------------------------------------------
		template<>
		inline void cTileStoreDense1::TypedRemove(const ecs::cEntityWithData& ed, const glm::ivec2& oldPos)
		{
			mCellEntities.View().Clear(oldPos.x, oldPos.y);
		}

		//---------------------------------------------------------------------------
		template<>
		inline void cTileStoreDenseN::TypedRemove(const ecs::cEntityWithData& ed, const glm::ivec2& oldPos)
		{
			auto& v = mCellEntities(oldPos);
			std::remove(v.begin(), v.end(), ed);
		}

		//---------------------------------------------------------------------------
		template<>
		inline void cTileStoreSparse1::TypedRemove(const ecs::cEntityWithData& ed, const glm::ivec2& oldPos)
		{
			mCellEntities.View().Clear(oldPos.x, oldPos.y);
		}

		//---------------------------------------------------------------------------
		template<>
		inline void cTileStoreSparseN::TypedRemove(const ecs::cEntityWithData& ed, const glm::ivec2& oldPos)
		{
			auto& v = mCellEntities(oldPos);
			std::remove(v.begin(), v.end(), ed);
		}

		//---------------------------------------------------------------------------
		template<>
		inline void cTileStoreDense1::TypedAdd(const ecs::cEntityWithData& ed, const glm::ivec2& newPos)
		{
			mCellEntities(newPos) = ed;
		}

		//---------------------------------------------------------------------------
		template<>
		inline void cTileStoreDenseN::TypedAdd(const ecs::cEntityWithData& ed, const glm::ivec2& newPos)
		{
			mCellEntities(newPos).push_back(ed);
		}

		//---------------------------------------------------------------------------
		template<>
		inline void cTileStoreSparse1::TypedAdd(const ecs::cEntityWithData& ed, const glm::ivec2& newPos)
		{
			mCellEntities(newPos) = ed;
		}

		//---------------------------------------------------------------------------
		template<>
		inline void cTileStoreSparseN::TypedAdd(const ecs::cEntityWithData& ed, const glm::ivec2& newPos)
		{
			mCellEntities(newPos).push_back(ed);
		}


		
	}
}