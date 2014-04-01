#include "InputKey.h"

#include <core/app/sdlapp.h>
#include <ecs/ecs.h>
#include <rl/components/components.h>

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
				
				// TODO: make it more generic
				auto& bindings = mainecs()->TagusToEntities("Player")->second->second.Component< cmp::cControllerPlayer>()->mBindings;
				if (evt.state == 1)
				{
					if (!bindings.empty())
						for (const auto& binding : bindings)
						{
							if (binding.first == evt.keysym.scancode)
							{
								binding.second();
								// tODO: after turn system, mEvtHandleOnKeyboard.Accept(false);
								break;
							}
						}
				}
			}
		}
	}
}