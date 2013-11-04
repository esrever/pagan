import os

files = [f for f in os.listdir('.') if os.path.isfile(f) and f.endswith("Action.h")]
incdata = "\n".join(["#include \"" + f + "\"" for f in files]);
regdata = "\n".join(["\t\tzMgr.AddAction(\"" + f[:-8] +"\",&pgn::action::" + f[:-8] + ");" for f in files]);

fp = open("actions.h", "w")
fp.write("#pragma once\n")
#fp.write(incdata)

hdata = """

namespace pgn
{
    class cActionMgr;
    void RegisterAllActions(cActionMgr& zMgr);
}
"""
fp.write(hdata)
fp.close()

cppdata = """#include "actions.h"
#include "ecs/ActionMgr.h"\n
"""  + incdata + """

namespace pgn
{
    void RegisterAllActions(cActionMgr& zMgr) \n\t{\n%s\n\t}
}
"""%regdata

fp = open("actions.cpp", "w")
fp.write(cppdata)
fp.close()