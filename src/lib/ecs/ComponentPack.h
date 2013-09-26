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
		// Fetch the data pointer from the component storage
		void Receive<cTypedComponentAddedEventData<T>>(cEvent<cTypedComponentAddedEventData<T>>::signal_data_type data);
		void Receive<cTypedRemoveComponentEventData<T>>(cEvent<cTypedRemoveComponentEventData<T>>::signal_data_type data);
	};
}