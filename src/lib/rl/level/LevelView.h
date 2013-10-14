#pragma once

#include <rl/events/events.h>

namespace pgn
{
	//! Type-specific views of a map, e.g. visibility, obstacle, movecost, asciirep, atmo, etc.
	template<class T>
	class cLevelView : public cArray2D<T::datatype>,
					   public cEventReceiver< cTileInLevelChangedEventData >,
					   public cEventReceiver< cLevelCreatedEventData >,
					   public cEventReceiver< cLevelDestroyEventData >
	{
		//TODO: have a default value for the nonexistent. Defaults in components & ctors! configurable defaults?
		public:
			void SetParentView(std::weak_ptr<const cComponentBase> zParent);
			void Receive( const cTileInLevelChangedEventData& zData);
			void Receive( const cLevelCreatedEventData& zData);
			void Receive( const cLevelDestroyEventData& zData);
		private:
			std::weak_ptr<const cComponentBase> mParentView; 
	};

	//------------------------------------------------------------------------------
	template<class T> void cLevelView<T>::Receive( const cTileInLevelChangedEventData& zData)
	{
		auto& ecs = ECS;
		auto comp = GetComponent(*std::get<0>(zData.data), cComponent<cLevel>::TypeIndex());
		if(comp)
		{
			const auto& loc = *std::get<1>(zData.data);
			auto& data = (*this)(loc.mCoords);
			// TODO: update
		}
	}

	//------------------------------------------------------------------------------
	template<class T> void cLevelView<T>::Receive( const cLevelCreatedEventData& zData)
	{
		// TODO: find we are actually assigned to the level being created
		mData.resize() 
		assert(false);
	}

	//------------------------------------------------------------------------------
	template<class T> void cLevelView<T>::Receive( const cLevelDestroyEventData& zData)
	{
		// TODO: find we are actually assigned to the level being destroyed
		mData.clear();
		assert(false);
	}

	//------------------------------------------------------------------------------
	template<class T> void cLevelView<T>::SetParentView(std::weak_ptr<const cComponentBase> zParent)
	{
		mParentView = zParent;
	}
}