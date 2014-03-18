#include "InputKey.h"

namespace pgn
{
	namespace ecs
	{
		namespace sys
		{
			//-------------------------------------------------------------------------
			cInputKey::cInputKey() :INIT_EVT_MEMBER(cInputKey, Keyboard){}

			//-------------------------------------------------------------------------
			void cInputKey::OnKeyboard(const SDL_KeyboardEvent& evt)
			{
				if (!Active()) return;
				// TODO: implement
				assert(false);
			}
		}
	}
}