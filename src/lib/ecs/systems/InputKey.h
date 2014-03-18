#pragma once

#include <core/event/Event.h>
#include <ecs/system.h>

namespace pgn
{
	namespace ecs
	{
		namespace sys
		{
			class cInputKey : public cBase
			{
				public:
					cInputKey();
					void OnKeyboard(const SDL_KeyboardEvent& evt);
				protected:
					virtual void SerializeOut(node_type& writer) const {  }
					virtual size_t SerializeIn(const node_type& reader)  { return 0; };
				private:
					DECL_EVT_MEMBER(Keyboard);
			};
		}
	}
}