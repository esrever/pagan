from rl_events import *
import sys

"""Usage: gen_system name ecc [listeners/desc]
    name:           name of class
    ecc:            event type
    queries/desc:   what is this going to be used for / who listens (optional)
"""

header_format = """
#pragma once
#include <ecs/SystemBase.h>
#include <rl/events/events.h>

namespace pgn
{
    namespace sys
    {
        class c%s : public cSystemBase
        {
        public:
            c%s():mProcess(Simple::slot(this, &c%s::Process)){}
        private:
            cEventHandler<evt::c%s> mProcess;
            void Process( %s);
        };
    }
}
"""

source_format = """
#include "%s.h"

namespace pgn
{
    void sys::c%s::Process( %s )
    {
        assert(false);
        auto q = ECS.mSystemMgr->GetQuery("which_query");
        for( e : q.Entities())
        {
        
        }
    }
}
"""

def generate_system_header( name, evt, args):
    return header_format%(name,name,name,evt,args)

def generate_system_source( name, evt, args):
    return source_format%(name,name,args)

def generate_system( name, evt):
    args = []

    for e in events:
        ecc = evt2camelcase(e[0])
        if evt == ecc:
            args = ", ".join(e[1]) 
            break
        
    s = generate_system_source(name,evt, args)
    h = generate_system_header(name,evt, args)
    
    write_to_file( "../systems/%s.h"%name,h);
    write_to_file( "../systems/%s.cpp"%name,s);

    
#name = sys.argv[1]
#ecc = sys.argv[2]

for (name, ecc) in [("DoorOpen","MehMeh"), ("Baa","DohDoh")]:
    generate_system( name, ecc)