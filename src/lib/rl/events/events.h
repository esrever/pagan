#pragma once

#include <string>
#include <glm/glm.hpp>
#include <ecs/EntityData.h>

namespace oxygine
{
	struct cKeyState;
}

namespace pgn
{
	namespace evt
	{
		enum eRL
		{
			LEVEL_CREATE = eBasicECS::num,
			LEVEL_DESTROY,
			LEVEL_LOAD,
			LEVEL_UNLOAD,
			LEVEL_ENTER,
			LEVEL_LEAVE,
			EXIT_APPLICATION,
			LOG,
			ACTION_IDLE,
			ACTION_MOVE_ADJ,
			DOOR_OPEN,
			DOOR_CLOSE,
            KEY_STATE
		};		

		typedef cEvent<size_t(eRL::LEVEL_CREATE), cEntityWithData> cLevelCreate;
		typedef cEvent<size_t(eRL::LEVEL_DESTROY), cEntityWithData> cLevelDestroy;
		typedef cEvent<size_t(eRL::LEVEL_LOAD), cEntityWithData> cLevelLoad;
		typedef cEvent<size_t(eRL::LEVEL_UNLOAD), cEntityWithData> cLevelUnload;
		typedef cEvent<size_t(eRL::LEVEL_ENTER), cEntityWithData, cEntityWithData> cLevelEnter;
		typedef cEvent<size_t(eRL::LEVEL_LEAVE), cEntityWithData, cEntityWithData> cLevelLeave;
		typedef cEvent<size_t(eRL::EXIT_APPLICATION), void> cExitApplication;
		typedef cEvent<size_t(eRL::LOG), const std::string&,const std::string&> cLog;
		typedef cEvent<size_t(eRL::ACTION_IDLE), cEntityWithData> cActionIdle;
		typedef cEvent<size_t(eRL::ACTION_MOVE_ADJ), cEntityWithData,const glm::ivec2&> cActionMoveAdj;
		typedef cEvent<size_t(eRL::DOOR_OPEN), cEntityWithData,cEntityWithData> cDoorOpen;
		typedef cEvent<size_t(eRL::DOOR_CLOSE), cEntityWithData,cEntityWithData> cDoorClose;
		typedef cEvent<size_t(eRL::KEY_STATE), const int, const oxygine::cKeyState&> cKeyState;
	}

	//-----------------------------------------------------------------
	//! Represents an action of an entity or a system.
	template< evt::eRL N, class... Args>
	class cAction
	{
		public:
			//! runs the action - nobody listens
			static bool Run(Args...);
			//! The event that others can listen to
			static void Event(Args...);

			inline static void RunEvent(Args... args)
			{
				if ( Run( args... ) )
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

		template<class T>
		static cActionBindings AddFrom()
		{
			const std::type_info& ti = typeid(T);
			mActionTypes[std::string(ti.name())] = &T::RunEvent;
			return cActionBindings();
		}

		static const action_map_type& Get() { return mActionTypes; }
	private:
		static action_map_type mActionTypes;
	};

	//! Specialisation, usable for special handling of no-argument functions. perhaps if they are ADDED to the other actions, so I don't have to create PLAYER_MOVE_NORTH events
	template< evt::eRL N>
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
		static cActionBindings mActionBinding;
	};
	template< evt::eRL N>
	cActionBindings cAction<N>::mActionBinding = cActionBindings::AddFrom<cAction<N> >();

	typedef cAction<(evt::eRL::LEVEL_CREATE), cEntityWithData> cActionLevelCreate;
	typedef cAction<(evt::eRL::LEVEL_DESTROY), cEntityWithData> cActionLevelDestroy;
	typedef cAction<(evt::eRL::LEVEL_LOAD), cEntityWithData> cActionLevelLoad;
	typedef cAction<(evt::eRL::LEVEL_UNLOAD), cEntityWithData> cActionLevelUnload;
	typedef cAction<(evt::eRL::LEVEL_ENTER), cEntityWithData, cEntityWithData> cActionLevelEnter;
	typedef cAction<(evt::eRL::LEVEL_LEAVE), cEntityWithData, cEntityWithData> cActionLevelLeave;
	typedef cAction<(evt::eRL::EXIT_APPLICATION)> cActionExitApplication;
	typedef cAction<(evt::eRL::LOG), const std::string&, const std::string&> cActionLog;
	typedef cAction<(evt::eRL::ACTION_IDLE), cEntityWithData> cActionActionIdle;
	typedef cAction<(evt::eRL::ACTION_MOVE_ADJ), cEntityWithData, const glm::ivec2&> cActionActionMoveAdj;
	typedef cAction<(evt::eRL::DOOR_OPEN), cEntityWithData, cEntityWithData> cActionDoorOpen;
	typedef cAction<(evt::eRL::DOOR_CLOSE), cEntityWithData, cEntityWithData> cActionDoorClose;
	typedef cAction<(evt::eRL::KEY_STATE), const int, const oxygine::cKeyState&> cActionKeyState;
}