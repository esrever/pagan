#pragma once

#include <ssignal.h>

/*
	Usage: 

	Receivers of T events:
		inherit from pgn::cEventListener<T> 
		implement Receive<T>(cEvent<T>::signal_data_type data)
		
		call foo.cEventListener<T>::Subscribe()
		...listen...
		let be destroyed, or call foo.cEventListener<T>::Unsubscribe()

	Events T:
		cEvent<T>::emit( Tinst )
*/

#define DECL_EVENT(N, T) struct c##N##EventData{ T data; c##N##EventData(){} c##N##EventData(const T &v):data(v){} };
#define EMIT_EVENT(N, V) pgn::cEvent<c##N##EventData>::emit(c##N##EventData( V ));

namespace pgn
{
	//!Event class
	template<class evt_data_type>
	class cEvent
	{
		public:
			typedef const evt_data_type& signal_data_type;
			typedef Gallant::Signal1<signal_data_type> signal_type;
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