#include "Keyboard.h"

#include <SDL_Events.h>
#include <SDL_scancode.h>
#include <Input.h>
#include <ssignal.h>

#include <rl/events/events.h>

typedef Simple::Signal<void(const SDL_KeyboardEvent& evt)> key_evt_sig;
static key_evt_sig sig;

namespace pgn
{
	struct cKeyCallback
	{
		public:
			static cKeyboard * msKeyMgr;
			static void OnKey(const int key, const oxygine::cKeyState& state)
			{
				if (msKeyMgr)
					msKeyMgr->OnKey(key, state);
			}
	};
	cKeyboard * cKeyCallback::msKeyMgr(nullptr);


	cKeyboard::cKeyboard()
	{
		// hook to oxygine key events
		cKeyCallback::msKeyMgr = this;
		oxygine::Input::instance.setKeyCallback(&cKeyCallback::OnKey);
		
		// Initialize keys
		mKeyStates.resize(SDL_NUM_SCANCODES);
	}

	cKeyboard::~cKeyboard()
	{
		cKeyCallback::msKeyMgr = nullptr;
		mKeyStates.clear();
	}

	const oxygine::cKeyState& cKeyboard::KeyState(const int key) const
	{ 
		return mKeyStates.at(key); 
	}

	void cKeyboard::OnKey(const int key, const oxygine::cKeyState& state)
	{
		mKeyStates.at(key) = state;
		// emit signal
		cActionKeyState::RunEvent(key, state);
	}
}