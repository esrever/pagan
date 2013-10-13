#pragma once

#include <rl/events/events.h>

namespace pgn
{
	//! Type-specific views of a map, e.g. visibility, obstacle, movecost, asciirep, atmo, etc.
	template<class T>
	class cLevelView : public cEventReceiver< cTileInLevelChangedEventData >,
					   public cEventReceiver< cLevelCreatedEventData >,
					   public cEventReceiver< cLevelDestroyEventData >
	{
		public:
			//! Access data at a location
			const T& operator()(size_t zX, size_t zY) const;
			
			//! Get raw dense data 
			//TODO: have a default value for the nonexistent. Defaults in components & ctors! configurable defaults?
			const T * Raw(size_t& zWidth, size_t& zHeight) const; 
		private:
	};
}