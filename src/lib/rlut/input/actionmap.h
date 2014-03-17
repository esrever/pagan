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
                void SetActive(bool v)  {   mActive = v;    }
                bool Active() const     {   return mActive; }
                
				void AddBindings(const std::map<std::string, std::string>& b);
                void OnKeyboard(const SDL_KeyboardEvent& evt);
                
            private:
                std::map<int, cECS::cActionFunc> mBindings;
                bool							 mActive;
                
				DECL_EVT_MEMBER(Keyboard);
        };
    }
    
    DECL_SERIALIZE_INTERFACE(rl::cActionMap);
}