events = [ ("level created",        ["cEntityWithData"]                                     ,False ) ,
           ("level destroy",        ["cEntityWithData"]                                     ) ,
           ("level loaded",         ["cEntityWithData"]                                     ) ,
           ("level unload",         ["cEntityWithData"]                                     ) ,
           ("exit application",     ["void"]                                                ) ,
           ("log",                  ["const std::string&", "const std::string&"]            ) ,
           ("action idle",          ["cEntityWithData"]                                     ) ,
           ("action move adj",      ["cEntityWithData", "const glm::ivec2&"]                ) ,
           ("action door open",     ["cEntityWithData","cEntityWithData"]                   ) ,
           ("action door close",    ["cEntityWithData","cEntityWithData"]                   ) ,
           ("door opened",          ["cEntityWithData"]                                     ) ,
           ("door closed",          ["cEntityWithData"]                                     ) ,
           ("tile in level changed",["cEntityWithData"]                                     ) ,
           ]

typedefs = []
evthandler_vars = []
evthandler_funcs = []

def write_to_file( fname, text):
    fp = open(fname,'w')
    fp.write(text)
    fp.close()

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

def generate_system( name, evt, args):
    s = generate_system_source(name,evt, args)
    h = generate_system_header(name,evt, args)
    
    write_to_file( "../systems/%s.h"%name,h);
    write_to_file( "../systems/%s.cpp"%name,s);

    
import sys
generate_system( sys.argv[1].capitalize(), sys.argv[2], sys.argv[3])