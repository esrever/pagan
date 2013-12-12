#include "KeyboardHandle.h"

#include <Input.h>

#include <ecs/Event.h>
#include <ecs/Component.h>
#include <ecs/EntityData.h>

#include <rl/components/KeyActionMapper.h>

namespace pgn
{
	namespace sys
	{
		cKeyboardHandle::cKeyboardHandle()
		:mOnKey(Simple::slot(this, &cKeyboardHandle::OnKey))
		{

		}

		void cKeyboardHandle::OnKey(const int key, const oxygine::cKeyState& state)
		{ 

			// Debug key presses
			//std::cout << boost::str(boost::format("%s - %s - %d\n")% (state.mPressed ? "pressed" : "released")%(state.mRepeat ? "repeat" : "no-repeat")%key );

			for (auto e : mQueryKeyActionMapper->Entities())
			{
				// Get the action mapper
				std::shared_ptr< cComponent<cmp::cKeyActionMapper>> kam_ptr;
				auto ec = ECS.mEntityMgr->GetEntityData().find(e);
				assert(ec != ECS.mEntityMgr->GetEntityData().end());
				ec->second.mComponents.GetComponent(kam_ptr);
				cmp::cKeyActionMapper& kam = kam_ptr->mData;
				
				// if pressed
				if (state.mPressed)
				{
					// if action is found
					auto it = kam.mActions.find(key);
					if (it != kam.mActions.end())
						// execute action!
						it->second();
				}
			}

			for (auto e : mQueryDevConsole->Entities())
			{
				// TODO: ... 
			}
		}
	}
}