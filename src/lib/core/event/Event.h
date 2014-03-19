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

#define DECL_EVT_MEMBER( N) public: pgn::cEventHandler<pgn::evt::c##N##> m##EvtHandleOn##N##; private:
#define INIT_EVT_MEMBER(T, N) mEvtHandleOn##N##( Simple::slot(this, &##T##::On##N ))

#define DECL_EVT0( T ) struct c##T{\
	typedef std::function<void()> cb_type; \
	typedef Simple::Signal<void()> sig_type; \
	static sig_type& Sig() { static sig_type mSig; return mSig; }}

#define DECL_EVT1( T, A0) struct c##T{\
	typedef std::function<void(const A0 &)> cb_type; \
	typedef Simple::Signal<void(const A0 &)> sig_type; \
	static sig_type& Sig() { static sig_type mSig; return mSig; }}

#define DECL_EVT2( T, A0, A1) struct c##T{\
	typedef std::function<void(const A0 &, const A1 &)> cb_type; \
	typedef Simple::Signal<void(const A0 &, const A1 &)> sig_type; \
	static sig_type& Sig() { static sig_type mSig; return mSig; }}

#define DECL_EVT3( T, A0, A1, A2) struct c##T{\
	typedef std::function<void(const A0 &, const A1 &, const A2 &)> cb_type; \
	typedef Simple::Signal<void(const A0 &, const A1 &, const A2 &)> sig_type; \
	static sig_type& Sig() { static sig_type mSig; return mSig; }}

namespace pgn
{
	template<class E>
	class cEventHandler
	{
	public:

		cEventHandler&  operator=   (const cEventHandler& eh)
		{
			E::Sig() -= mId;
			mId = eh.mId;
			mActivate = eh.mActivate;
			mCallback = eh.mCallback;
			return *this;
		}

		cEventHandler(typename E::cb_type zCallback)
		:mCallback(zCallback)
		{
			mActivate = true;
			mId = E::Sig() += zCallback;
		}

		~cEventHandler()
		{
			E::Sig() -= mId;
			mId = 0;
		}

		void Activate(bool zOk) 
		{
			if (mActivate && zOk)
				return;
			else if (!(mActivate || zOk))
				return;
			else if (mActivate)
			{
				mActivate = false;
				E::mSig -= mId;
			}
			else // ( zOk)
			{
				mId = E::Sig() += mCallback;
			}
		}

	private:
		cEventHandler(const cEventHandler& eh)
		{
			mId = eh.mId;
		}

	private:
		unsigned mId;
		bool mActivate;
		typename E::cb_type mCallback;
	};

	//! Event-specific stuff
	namespace evt
	{
		DECL_EVT1(Keyboard, SDL_KeyboardEvent);
		DECL_EVT1(MouseMotion, SDL_MouseMotionEvent);
		DECL_EVT1(MouseButton, SDL_MouseButtonEvent);
		DECL_EVT1(MouseWheel, SDL_MouseWheelEvent);
		DECL_EVT0(ApplicationExit);
		DECL_EVT1(Window, SDL_WindowEvent);
	}

	/**
	Action registering: hardcoded list plus automatic from events

	zMgr.AddAction("AppHelp", &evt::AppHelp);
	for (auto x : cActionBindings::Instance().Get())
	zMgr.AddAction(x.first, x.second);
	*/
	/*
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
	*/
}