#pragma once

#include <core/container/Array2D.h>
#include <ecs/EntityData.h>
#include <rl/components/Level.h>
#include <rl/components/LevelPosition.h>

namespace pgn
{
	//! View of a map templated on the proces
	template<class T>
	class cMapView
	{
	public:
		void SetDefault(const T::value_type& v) { mDefaultValue = v; }
		void Regenerate(cEntityWithData zLevel);
		void Update(size_t x, size_t y, cEntityWithData zTile);
	private:
		T::value_type					 mDefaultValue;
		cEntityWithData					 mLevel;
		typename cArray2D<T::value_type> mView;
	};

	//-----------------------------------------------------------
	template<class T>
	void cMapView<T>::Regenerate(cEntityWithData zLevel)
	{
		mLevel = zLevel;
		auto lvl_ptr = mLevel->second.mComponents.GetComponent< cmp::cLevel>();
		// resize
		mView.Resize(lvl_ptr->mLayoutData.Width(), lvl_ptr->mLayoutData.Height());
		// initialize with default values
		mView.Fill(0, 0, mView.Width(), mView.Height(), mDefaultValue);
		// call Update for each BG tile
		for (int i = 0; i < mView.Height(); ++i)
		{
			for (int j = 0; j < mView.Width(); ++j)
			{
				Update(j, i, lvl_ptr->mData.LookupEntity(j, i));
			}
		}
		// call Update for each FG tile
		for (auto ed : lvl_ptr->mData.mEntities)
		{
			auto pos_ptr = ed->second.mComponents.GetComponent<cmp::cLevelPosition>();
			if (pos_ptr)
			{
				const auto& pos = pos_ptr->mData.mPos;
				Update(pos.x, pos.y, ed);
			}
		}
	}
}