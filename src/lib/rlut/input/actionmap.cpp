#include "actionmap.h"

namespace pgn
{
	//---------------------------------------------------------------------------------------------------
	void SerializeOut(node_type& node, const rl::cActionMap& value)
	{
        
	}

	//---------------------------------------------------------------------------------------------------
	size_t SerializeIn(const node_type& node, rl::cActionMap& value)
	{
		size_t ret = 0;

		std::map<std::string, std::string> bindings;
		ret += SerializeIn(node, bindings);
		value.AddBindings(bindings);
		return ret;
	}

	namespace rl
	{
		cActionMap::cActionMap() :
			INIT_EVT_MEMBER(cActionMap, Keyboard)
		{
			// TODO: after turn system, set this to false
			mEvtHandleOnKeyboard.Activate(true);
		}

		cActionMap::cActionMap(const cActionMap& v) : mBindings(v.mBindings), INIT_EVT_MEMBER(cActionMap, Keyboard){}

        void cActionMap::OnKeyboard(const SDL_KeyboardEvent& evt)
        { 
            if(evt.state == 1)
            { 
                for(const auto& binding : mBindings)
                {
                    if(binding.first == evt.keysym.scancode)
                    {
                        binding.second();
						// tODO: after turn system, mEvtHandleOnKeyboard.Accept(false);
                        break;
                    }
                }
            }
        }

		void cActionMap::AddBindings(const std::map<std::string, std::string>& bindings)
		{
			// TODO: allow vectors of keys and special ones!
			for (const auto& kv : bindings)
			{
				mBindings[std::from_string<int>(kv.first)] = mainecs()->ActionFuncs(kv.second)->second;
			}
		}
	}
}