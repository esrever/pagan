import os

files = [f for f in os.listdir('.') if os.path.isfile(f) and f.endswith(".h") and (f != "systems.h")]
incdata = "\n".join(["#include \"" + f + "\"" for f in files]);
regdata = "\n".join(["\t\tzMgr.AddSystemType<pgn::sys::c" + f[:-2] + ">();" for f in files]);

fp = open("systems.h", "w")
fp.write("#pragma once\n")

hdata = """

namespace pgn
{
    class cSystemMgr;
    void RegisterAllSystems(cSystemMgr& zMgr);
}
"""
fp.write(hdata)
fp.close()

cppdata = """#include "systems.h"
#include "ecs/SystemMgr.h"
%s

namespace pgn
{
    void RegisterAllSystems(cSystemMgr& zMgr) \n\t{\n%s\n\t}
}
"""%(incdata,regdata)

fp = open("systems.cpp", "w")
fp.write(cppdata)
fp.close()