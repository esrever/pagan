#pragma once

#include "Event.h"

struct SDL_KeyboardEvent;

namespace pgn
{
	typedef std::function<void(const SDL_KeyboardEvent& evt)> key_evt_fun;

	struct cKeyState
	{
		unsigned short mMod;
		bool		   mPressed;
		bool		   mRepeat;
		
		cKeyState() :mMod(0), mPressed(false), mRepeat(false){}
	};

	class cKeyboardMgr
	{
		public:
			void Init();
			void Destroy();
			const cKeyState& KeyState(const int key) const { return mKeyStates.at(key); }
		private:
			cEventHandler<evt::cKeyEvt> mOnKey;
			void OnKey(const SDL_KeyboardEvent& evt);

		private:
			unsigned			   mCbId;
			std::vector<cKeyState> mKeyStates;
	};
}