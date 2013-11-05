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
		Add member  : cEventHandler<cXXXEvent> mEventHandlerXXX;
		Add function: void cFoo::HandleEvent(Args...)
		initialize  : :mEventHandlerXXX(Simple::slot(this, &Foo::HandleEvent))

	Events:
		enum class eEvent : size_t {
			LOCATION_CHANGED = 0,
			ENTITY_CREATED = 1
		};
	
	Emitting:
		cXXXEvent::mSig.emit(args...)

	Notes:
		Events are instantiated at first emission - don't generate events at compile time.
*/

namespace pgn
{
	template< size_t E, class... Args >
	struct cEvent
	{
		typedef Simple::Signal<void(Args...)> sigtype;
		typedef std::function<void(Args...)> functype;
		static sigtype mSig;
	};
	template< size_t E, class... Args >
	typename cEvent<E, Args...>::sigtype cEvent<E, Args...>::mSig;


	template<class E>
	class cEventHandler
	{
	public:

		cEventHandler&  operator=   (const cEventHandler& eh)
		{
			E::mSig -= mId;
			mId = eh.mId;
			return *this;
		}

		cEventHandler(typename E::functype zCallback)
		{
			mId = E::mSig += zCallback;
		}

		~cEventHandler()
		{
			E::mSig -= mId;
		}

	private:
		unsigned mId;
	};
}