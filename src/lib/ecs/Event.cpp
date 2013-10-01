#include "Event.h"
#include "EntityMgr.h"

namespace pgn
{
    void log_event_call(const std::string& s)
    {
		return ECS.mLog.Dbg(s);
    }
}