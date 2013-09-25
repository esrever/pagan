#pragma once

#include <ssignal.h>

namespace pgn
{
	//!Event class
	template<class evt_data_type>
	class cEvent
	{
		public:
			typedef const evt_data_type& signal_data_type;
			typedef Gallant::Signal1<signal_data_type> signal_type;
			typedef std::weak_ptr<signal_type> signal_wptr_type;
			typedef std::shared_ptr<signal_type> signal_sptr_type;
		public:
			static signal_type emit;
	};

	//! Instantiate the signal
	template<class evt_data_type> typename cEvent<evt_data_type>::signal_type cEvent<evt_data_type>::emit;

	//!Event receiver base class
	template<class evt_data_type>
	class cEventReceiver
	{
		public:
			cEventReceiver() {}
			virtual ~cEventReceiver(){ Unsubscribe();}

			void Subscribe();
			void Unsubscribe();
			virtual void Receive(typename cEvent<evt_data_type>::signal_data_type zData) {}
	};


	//-------------------------------------------
	template<class evt_data_type> 
	void cEventReceiver<evt_data_type> ::Subscribe()
	{
		cEvent<evt_data_type>::emit.Connect(this, &cEventReceiver<evt_data_type>::Receive);
	}

	template<class evt_data_type> 
	void cEventReceiver<evt_data_type>::Unsubscribe()
	{
		cEvent<evt_data_type>::emit.Disconnect(this, &cEventReceiver<evt_data_type>::Receive);
	}
}