#pragma once

namespace pgn
{
	// TODO: make versions for T0, <T0,T1>, <T0,T1,T2>, etc... up to 8?

	// Declare the typed events
	template<class T>
	struct cTypedComponentAddedEventData{ cEntity data; cTypedComponentAddedEventData(){} cTypedComponentAddedEventData(const cEntity &v):data(v){} };
	template<class T>
	struct cRemoveTypedComponentEventData{ cEntity data; cRemoveTypedComponentEventData(){} cRemoveTypedComponentEventData(const cEntity &v):data(v){} };

	template<class T>
	class cComponentPack : public cEventReceiver<cTypedComponentAddedEventData<T>>,
						   public cEventReceiver<cTypedRemoveComponentEventData<T>>
	{
		public:
			// TODO: get all entities that contain the required components
			cComponentPack();

			// Fetch the data pointer from the component storage
			void Receive<cTypedComponentAddedEventData<T>>(cEvent<cTypedComponentAddedEventData<T>>::signal_data_type data);
			void Receive<cTypedRemoveComponentEventData<T>>(cEvent<cTypedRemoveComponentEventData<T>>::signal_data_type data);

		private:
			std::set<cEntity> mEntitiesWithPack;
	};

	//------------------------------------------------------------------------------------
	template<class T>
	cComponentPack::cComponentPack()
	{
		// TODO: for 1 arg, get whole map, for 2,3,N args, set_intersect
		// Perhaps write everything as functors and call recursively
		auto mapc = cComponentStorage<T>::Components();
		for(auto i : mapc)
			mEntitiesWithPack.insert(i->first);
	}
}