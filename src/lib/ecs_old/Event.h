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

		cEventHandler(const cEventHandler& eh)
		{
			mId = eh.mId;
		}

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

	//-----------------------------------------------------------------
	//! Represents an action of an entity or a system.
	template< size_t N, class... Args>
	class cAction
	{
	public:
		//! runs the action - nobody listens
		static bool Run(Args...);
		//! The event that others can listen to
		static void Event(Args...);

		inline static void RunEvent(Args... args)
		{
			if (Run(args...))
			{
				Event(args...);
				cEvent< N, Args...>::mSig.emit(args...);
			}
		}
	};

	//! Automatic storage for 0-argument functions
	struct cActionBindings
	{
		typedef std::function<void(void)> action_type;
		typedef std::map< std::string, action_type> action_map_type;

		static cActionBindings& Instance() { static cActionBindings inst = cActionBindings(); return inst; }

		template<class T>
		cActionBindings& AddFrom()
		{
			const std::type_info& ti = typeid(T);
			mActionTypes[std::string(ti.name())] = &T::RunEvent;
			return *this;
		}

		const action_map_type& Get() { return mActionTypes; }
	private:
		action_map_type mActionTypes;
	};

	//! Specialisation, usable for special handling of no-argument functions. perhaps if they are ADDED to the other actions, so I don't have to create PLAYER_MOVE_NORTH events
	template< size_t N>
	class cAction<N>
	{
	public:
		//! runs the action - nobody listens
		static bool Run();
		//! The event that others can listen to
		static void Event();

		inline static void RunEvent()
		{
			mActionBinding;  // I just need to instantiate it
			if (Run())
			{
				Event();
				cEvent< N>::mSig.emit();
			}
		}
	private:
		static cActionBindings& mActionBinding;
	};
	template< size_t N>
	cActionBindings& cAction<N>::mActionBinding = cActionBindings::Instance().AddFrom<cAction<N> >();
}