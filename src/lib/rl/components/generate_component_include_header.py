import os

files = [f for f in os.listdir('.') if os.path.isfile(f) and f.endswith(".h") and (f != "components.h")]
incdata = "\n".join(["#include \"" + f + "\"" for f in files]);
regdata = "\n".join(["\t\tzMgr.AddComponentType<pgn::c" + f[:-2] + ">();" for f in files]);

fp = open("components.h", "w")
fp.write("#pragma once\n")
fp.write(incdata)

hdata = """

namespace pgn
{
    class cEntityMgr;
    void RegisterAllComponents(cEntityMgr& zMgr);
}
"""
fp.write(hdata)
fp.close()

cppdata = """#include "components.h"
#include "ecs/EntityMgr.h"

namespace pgn
{
    void RegisterAllComponents(cEntityMgr& zMgr) \n\t{\n%s\n\t}
}
"""%regdata

fp = open("components.cpp", "w")
fp.write(cppdata)
fp.close()