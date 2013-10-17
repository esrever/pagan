#pragma once

#include <boost/format.hpp>

#include <ssignal.h>
#include <core/util/singleton.h>
#include <core/util/conversions.h>

#include "ecs_config.h"
//#include "Entity.h"
#include "ComponentBase.h"

/*	
	Usage: 

	Receivers of T events:
		inherit from pgn::cEventListener<T> 
		implement Receive<T>(cEvent<T>::signal_data_type data)

	Events T:
		cEvent<T>::emit( Tinst )
	Notes:
		Events are instantiated at first emission - don't generate events at compile time.
		do a less clunky emit with bind
			emit(eEvent, arg0, arg1, ...)
				auto handle = std::bind( event_handle2, _1, arg0, arg1 ); // Bind the arguments
				for_each recv in receivers[eEvent], handle(recv)
			Subscribe( vector<eEvent> ) : 
				
*/

namespace pgn
{
	//! for debugging the event calls
	void log_event_call(const std::string& s);

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

	//!Event receiver base class
	template<class evt_data_type>
	class cEventReceiver
	{
		public:
			cEventReceiver() {Subscribe();}
			virtual ~cEventReceiver(){ Unsubscribe();}

			void Subscribe();
			void Unsubscribe();
			virtual const std::string ReceiverName() const {return boost::str(boost::format("%x")% this );}
			virtual void Receive(typename cEvent<evt_data_type>::signal_data_type zData) {}
	};


	//-------------------------------------------
	template<class evt_data_type> 
	void cEventReceiver<evt_data_type> ::Subscribe()
	{
		log_event_call(boost::str(boost::format("%s subscribes to %s")% ReceiverName().c_str() %typeid(evt_data_type).name() ));
		pgn::cSingleton<cEvent<evt_data_type>>::Instance().emit.Connect(this, &cEventReceiver<evt_data_type>::Receive);		
	}

	//-------------------------------------------
	template<class evt_data_type> 
	void cEventReceiver<evt_data_type>::Unsubscribe()
	{
		log_event_call(boost::str(boost::format("%s unsubscribes from %s")% ReceiverName().c_str() %typeid(evt_data_type).name() ));
		pgn::cSingleton<cEvent<evt_data_type>>::Instance().emit.Disconnect(this, &cEventReceiver<evt_data_type>::Receive);
	}

	//-------------------------------------------
	//! create a string out of an emitted event
	template <class evt_data_type>
	inline std::string evt_string(const std::string& evtName, const evt_data_type& evtData)
	{
		return boost::str(boost::format("Emitting event %s with data: %s")%evtName.c_str()%pgn::to_string(evtData));
	}
	template <class evt_data_type>
	inline std::string evt_string(const std::string& evtName)
	{
		return boost::str(boost::format("Emitting non-data event %s")%evtName.c_str());
	}

	//-------------------------------------------
	//! emit event 
	template<class T>
	void emit_event(const typename T::data_type & zVal)
	{
		const std::type_info& ti = typeid(T);
		std::string s = evt_string<typename T::data_type>( ti.name(), zVal );
		log_event_call(s);

		T evd;
		evd.data = zVal;
		pgn::cSingleton<pgn::cEvent<T>>::Instance().emit(zVal);
	}
	template<class T>
	void emit_event()
	{
		const std::type_info& ti = typeid(T);
		std::string s = evt_string<typename T::data_type>( ti.name() );
		log_event_call(s);
		pgn::cSingleton<pgn::cEvent<T>>::Instance().emit(T());
	}
}