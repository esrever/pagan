#include "KeyboardMgr.h"

#include <SDL_Events.h>
#include <SDL_scancode.h>
#include <Input.h>
#include <ssignal.h>

typedef Simple::Signal<void(const SDL_KeyboardEvent& evt)> key_evt_sig;
static key_evt_sig sig;

namespace pgn
{
	// TODO: The below is crap and wont work. I need to hook sendKeyEvt to a local function. 
	void cKeyboardMgr()
		:mOnKey(Simple::slot(this, &cKeyboardMgr::OnKey))
	{
		// hook to oxygine key events
		auto func = std::bind(&oxygine::Input::sendKeyEvent, &oxygine::Input::instance, std::placeholders::_1);
		
		// Initialize keys
		mKeyStates.resize(SDL_NUM_SCANCODES);
	}

	void cKeyboardMgr::Destroy()
	{
		sig -= mCbId;
		mKeyStates.clear();
	}

	void cKeyboardMgr::OnKeyStateChange(const SDL_KeyboardEvent& evt)
	{
		auto& state = mKeyStates[int(evt.keysym.scancode)];
		state.mMod = evt.keysym.mod;
		state.mRepeat = evt.repeat != 0;
		state.mPressed = evt.state == SDL_PRESSED;
	}
}