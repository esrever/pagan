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
        void cActionMap::OnKeyboard(SDL_KeyboardEvent evt)
        {       
            if( Active() && (evt.state == 1))
            {
                for(const auto& binding : mBindings)
                {
                    if(binding.first == evt.keysym.sym)
                    {
                        binding.second();
                        SetActive(false);
                        break;
                    }
                }
            }
        }

		void cActionMap::AddBindings(const std::map<std::string, std::string>& bindings)
		{
			// TODO: from the strings, read the appr stuff
			for (const auto& kv : bindings)
			{
				// Single key
				if (kv.first.size() == 1)
				{

				}
				else // special key shortcuts! endsswith arrow, Ecs, space, enter, F1, etc
				{
					
				}
			}
		}
	}
}