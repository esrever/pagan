#include "KeyboardMgr.h"

#include <SDL_Events.h>
#include <SDL_scancode.h>
#include <Input.h>
#include <ssignal.h>

typedef Simple::Signal<void(const SDL_KeyboardEvent& evt)> key_evt_sig;
static key_evt_sig sig;

namespace pgn
{
	struct cKeyCallback
	{
		public:
			static cKeyboardMgr * msKeyMgr;
			static void OnKey(const int key, const oxygine::cKeyState& state)
			{
				if (msKeyMgr)
					msKeyMgr->OnKey(key, state);
			}
	};
	cKeyboardMgr * cKeyCallback::msKeyMgr(nullptr);


	cKeyboardMgr::cKeyboardMgr()
	{
		// hook to oxygine key events
		cKeyCallback::msKeyMgr = this;
		oxygine::Input::instance.setKeyCallback(&cKeyCallback::OnKey);
		
		// Initialize keys
		mKeyStates.resize(SDL_NUM_SCANCODES);
	}

	cKeyboardMgr::~cKeyboardMgr()
	{
		cKeyCallback::msKeyMgr = nullptr;
		mKeyStates.clear();
	}

	const oxygine::cKeyState& cKeyboardMgr::KeyState(const int key) const
	{ 
		return mKeyStates.at(key); 
	}

	void cKeyboardMgr::OnKey(const int key, const oxygine::cKeyState& state)
	{
		mKeyStates.at(key) = state;
		// emit signal
		evt::cKey::mSig.emit(key, state);
	}
}