#include "Event.h"
#include "ecs.h"

namespace pgn
{
    void log_event_call(const std::string& s)
    {
		return ECS.mLog.Dbg(s);
    }
}