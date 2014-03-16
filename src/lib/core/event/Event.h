#pragma once

#include <ssignal.h>
#include <map>

#include <SDL_events.h>

/**	
	Usage: 

	Receivers of T events:
		Add member  : cEventHandler<evt::cBlah> mHandleOnBlah;
			DECL_EVT_MEMBER(Blah)
		Add function: void cFoo::OnBlah(Args...)
		initialize  : :mEvtHandleOnBlah(Simple::slot(this, &Foo::OnBlah))
			INIT_EVT_MEMBER(Foo, Blah)

	Events:
		
		namespace evt
		{
			// event ids
			enum class eEventId : size_t {
				LocationChange=0,
				LevelEnter=1
			};


			// actions/events
			typedef cAction<size_t(LocationChange), cEntityWithData, cEntityWithData, const glm::ivec2&> cLocationChange;
		}
	

	Emitting actions/events:
		evt::cLocationChanged::Emit();			// just emit event
		evt::cLocationChanged::Run();			// just run the action
		evt::cLocationChanged::RunEvent();		// run action; if successful, emit event

	Notes:
		Events are instantiated at first emission - don't generate events at compile time.

	
*/

#define DECL_EVT_MEMBER( N) pgn::cEventHandler<pgn::evt::c##N##::evt_type> m##EvtHandleOn##N##;
#define INIT_EVT_MEMBER(T, N) mEvtHandleOn##N##( Simple::slot(this, &##T##::On##N ))

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
			mId = 0;
		}

	private:
		unsigned mId;
	};

	/**
	Action registering: hardcoded list plus automatic from events

	zMgr.AddAction("AppHelp", &evt::AppHelp);
	for (auto x : cActionBindings::Instance().Get())
	zMgr.AddAction(x.first, x.second);
	*/
	struct cActionBindings
	{
		typedef std::function<void(void)> action_type;
		typedef std::map< std::string, action_type> action_map_type;

		static cActionBindings& Instance();

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

	//-----------------------------------------------------------------
	//! Represents an action of an entity or a system.
	template< size_t N, class... Args>
	class cAction
	{
	public:
		typedef cEvent< N, Args...> evt_type;
		//! runs the action - nobody listens
		static bool Run(Args...);

		inline static void Emit(Args... args) { evt_type::mSig.emit(args...);  }
		inline static void RunEvent(Args... args)
		{
			if (Run(args...))
			{
				Emit(args...);
			}
		}
	};

	//! Specialisation, usable for special handling of no-argument functions. perhaps if they are ADDED to the other actions, so I don't have to create PLAYER_MOVE_NORTH events
	template< size_t N>
	class cAction<N>
	{
	public:
		typedef cEvent< N> evt_type;
		//! runs the action - nobody listens
		static bool Run();

		inline static void Emit() { evt_type::mSig.emit(); }
		inline static void RunEvent()
		{
			mActionBinding;  // I just need to instantiate it
			if (Run())
			{
				Emit();
			}
		}
	private:
		static cActionBindings& mActionBinding;
	};
	template< size_t N>
	cActionBindings& cAction<N>::mActionBinding = cActionBindings::Instance().AddFrom<cAction<N> >();

	//! Event-specific stuff
	namespace evt
	{
		enum class eCoreEventId : size_t {
			Keyboard = 1,
			MouseMotion,
			MouseWheel,
			MouseButton,
			ApplicationExit,
			Window
		};

		typedef cAction<size_t(eCoreEventId::Keyboard), SDL_KeyboardEvent> cKeyboard;
		typedef cAction<size_t(eCoreEventId::MouseMotion), SDL_MouseMotionEvent> cMouseMotion;
		typedef cAction<size_t(eCoreEventId::MouseWheel), SDL_MouseWheelEvent> cMouseWheel;
		typedef cAction<size_t(eCoreEventId::MouseButton), SDL_MouseButtonEvent> cMouseButton;
		typedef cAction<size_t(eCoreEventId::ApplicationExit)> cApplicationExit;
		typedef cAction<size_t(eCoreEventId::Window), SDL_WindowEvent> cWindow;
	}
}