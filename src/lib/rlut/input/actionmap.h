#pragma once

#include <core/event/Event.h>
#include <core/serialize/serialize.h>
#include <ecs/ecs.h>

namespace pgn
{
    namespace rl
    {
        class cActionMap
        {
            public:
                cActionMap();
				cActionMap(const cActionMap& v);
                
				void AddBindings(const std::map<std::string, std::string>& b);
                void OnKeyboard(const SDL_KeyboardEvent& evt);
                
            private:
                std::map<int, ecs::cActionFunc> mBindings;
                
				DECL_EVT_MEMBER(Keyboard);
        };
    }
    
    DECL_SERIALIZE_INTERFACE(rl::cActionMap);
}