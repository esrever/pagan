#pragma once

#include <Windows.h>

#include <boost/format.hpp>

#include <ssignal.h>
#include <core/util/singleton.h>
#include <core/util/conversions.h>

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

#define DECL_EVENT(N, T) struct c##N##EventData{ typedef T data_type; T data; c##N##EventData(){} c##N##EventData(const T &v):data(v){} };
#define EMIT_EVENT_DBG(N, V) { OutputDebugStringA( pgn::evt_string< c##N##EventData::data_type >( #N , V ).c_str());OutputDebugStringA( "\n" ); EMIT_EVENT_REL( N , V )}
#define EMIT_EVENT(N,V) emit_event< c##N##EventData >(V);

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
			signal_type emit;
	};

	//! Instantiate the signal
	//template<class evt_data_type> typename cEvent<evt_data_type>::signal_type cEvent<evt_data_type>::emit;

	//!Event receiver base class
	template<class evt_data_type>
	class cEventReceiver
	{
		public:
			cEventReceiver() {Subscribe();}
			virtual ~cEventReceiver(){ Unsubscribe();}

			void Subscribe();
			void Unsubscribe();
			virtual void Receive(typename cEvent<evt_data_type>::signal_data_type zData) {}
	};


	//-------------------------------------------
	template<class evt_data_type> 
	void cEventReceiver<evt_data_type> ::Subscribe()
	{
		pgn::cSingleton<cEvent<evt_data_type>>::Instance().emit.Connect(this, &cEventReceiver<evt_data_type>::Receive);
	}

	//-------------------------------------------
	template<class evt_data_type> 
	void cEventReceiver<evt_data_type>::Unsubscribe()
	{
		pgn::cSingleton<cEvent<evt_data_type>>::Instance().emit.Disconnect(this, &cEventReceiver<evt_data_type>::Receive);
	}

	//-------------------------------------------
	template <class evt_data_type>
	inline std::string evt_string(const std::string& evtName, const evt_data_type& evtData)
	{
		return boost::str(boost::format("Emitting event %s with data: %s")%evtName.c_str()%pgn::to_string(evtData));
	}

	template<class T>
	void emit_event(const typename T::data_type & zVal)
	{
		const std::type_info& ti = typeid(T);
		std::string s = evt_string<typename T::data_type>( ti.name(), zVal ) + "\n";
		OutputDebugStringA( s.c_str() );

		T evd;
		evd.data = zVal;
		pgn::cSingleton<pgn::cEvent<T>>::Instance().emit(evd);
	}
}