#include "Event.h"

namespace pgn
{
	cActionBindings& cActionBindings::Instance()
	{ 
		static cActionBindings inst = cActionBindings(); 
		return inst; 
	}
}