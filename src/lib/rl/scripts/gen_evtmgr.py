from rl_events import *

typedefs = []
evthandler_vars = []
evthandler_funcs = []
           
hev_includes = [ "<string>", "<glm/glm.hpp>", "<ecs/EntityData.h>"]

def create_enum_decl():
    lines_pref = ["enum eRL", "{"];
    lines_suff = ["};"];
    lines = [];
    for i in range(len(events)):
        e = events[i]
        e_name = e[0]
        line = evt2constant(e_name)
        if i==0:
            line += " = eBasicECS::num,";
        elif i!= (len(events)-1):
            line += ","
        lines.append(line)
    
    return lines_pref + pad_lines(lines,1) + lines_suff
    
    
def create_typedefs():
    lines = [];
    for i in range(len(events)):
        e = events[i]
        e_name = e[0]
        e_args = e[1]
        typedef = "c" + evt2camelcase(e_name)
        line = "typedef cEvent<size_t(eRL::%s), %s> %s;"%( evt2constant(e_name), ",".join([s for s in e_args]) ,typedef)
        lines.append(line)
        typedefs.append(typedef)
    
    return lines


#print "\n".join(create_enum_decl())
#print "\n".join(create_typedefs())


def generate_events_header():
    lines = ["#pragma once\n"];
    lines += [ "#include " + s for s in hev_includes]
    lines += ["\n"]
    
    pgnlines = []
    pgnlines += create_enum_decl()
    pgnlines += ["\n"]
    pgnlines += create_typedefs()
    pgnlines += ["\n"]
    for e in events:
        ecc = evt2camelcase(e[0])
        args = ", ".join(e[1])
        pgnlines += ["void On%s(%s);"%(ecc,args)]
    return lines + nswrap( "pgn", nswrap("evt", pgnlines))
    
    
    
def generate_events_source():
    sev_includes = [];
    lines = ["#include \"events.h\""];
    lines += [ "#include " + s for s in sev_includes]
    lines += ["\n"]
    
    sep = "//" + "".join(["-" for i in range(100)])
    pgnlines = []
    for e in events:
        ecc = evt2camelcase(e[0])
        args = []
        for i in range(len(e[1])):
            if e[1][i] != "void":
                args.append("%s arg%d"%(e[1][i], i))
        args = ", ".join(args)
        pgnlines += [sep, "void On%s(%s)"%(ecc,args), "{","\tassert(false);","}"]
    return lines + nswrap( "pgn", nswrap( "evt", pgnlines))
    
def generate_events_reg():
    sev_includes = [];
    lines = ["#include \"events.h\""];
    lines += [ "#include " + s for s in ["<rl/SystemMgrRL.h>"]]
    lines += ["\n"]
    
    pgnlines = ["void cSystemMgrRL::RegisterAllEvents()","{"]
    
    sep = "//" + "".join(["-" for i in range(100)])
    for e in events:
        ecc = evt2camelcase(e[0])
        pgnlines += ["\tevt::c%s::mSig += &evt::On%s;"%(ecc,ecc)]
        
    pgnlines += ["}"]
    return lines + nswrap( "pgn", pgnlines)
    
def generate_event_mgr_header():
    lines = ["#pragma once\n"];
    #lines += [ "#include " + s for s in h_includes]
    lines += [ "#include " + s for s in ["<rl/events/events.h>"]]
    
    clines = ["public:", "cEventMgrRL();","private:"]
    
    # create the variables
    for i in range(len(events)):
        e = events[i]
        e_name = e[0]
        ecc = evt2camelcase(e_name)
        var = "mOn%s"%ecc
        evthandler_vars.append(var)
        line = "cEventHandler<evt::c%s> %s;"%(ecc,var)
        clines.append(line)
        
    # create the functions
    for i in range(len(events)):
        e = events[i]
        e_name = e[0]
        e_args = e[1]
        ecc = evt2camelcase(e_name)
        args = ", ".join(e_args)
        func = "void On%s( %s)"%( ecc, args)
        evthandler_funcs.append(func)
        clines += [func + ";"]
        
    clines = nswrap("pgn", ["class cEventMgr","{"] + pad_lines(clines,1) + ["}"])
    
    return lines + ["\n"] + clines
    
def generate_event_mgr_source():
    lines += [ "#include " + s for s in ["\"EventMgrRL.h\"", "<ssignal.h>"]]
    
    sep = "//" + "".join(["-" for i in range(100)])
    
    #create initialisation lists
    pgnlines = [sep, "cEventMgrRL::cEventMgrRL():"]
    for i in range(len(events)):
        e_name = e[0]
        ecc = evt2camelcase(e_name)
        pgnlines.append( ":m%s(Simple::slot(this, &cSystemMgrRL::On%s))"%(ecc,ecc) )
    pgnlines += ["{}\n"]
    
    for i in range(len(events)):
        e = events[i]
        
        
def generate_event_handler_query_class( e):
    sep = "//" + "".join(["-" for i in range(100)])
    lines = ["",sep]
    
    e_name = e[0]
    e_args = e[1]
    
    ecc = evt2camelcase(e_name)
    args = ", ".join(e_args)
    
    lines += ["class cEventHandlerQuery_%s : public cEventHandlerQueryBase"%ecc, "{", "public:"];
    
    lines += ["\tcEventHandlerQuery_%s():mHandler(Simple::slot(this, &cEventHandlerQuery_%s::Handle)){}"%(ecc,ecc)]
    #lines += ["\tvoid SetQuery(cQueryExpressionSptr q) {mQuery = q;}"]
    lines += ["\tvoid Handle( %s );"%(args)]
    lines += ["virtual std::shared_ptr<cEventHandlerQueryBase> clone() const {return std::dynamic_pointer_cast<cEventHandlerQueryBase>(std::make_shared<cEventHandlerQuery_%s>(*this));}"%ecc]
    lines += ["private:"]
    lines += ["\tcEventHandler<evt::c%s> mHandler;"%ecc]
    #lines += ["\tcQueryExpressionSptr mQuery;"]
    lines += ["};"]
    
    return lines

def generate_event_handler_query_header():
    lines = ["#pragma once\n"];
    lines += [ "#include " + s for s in ["<ecs/Event.h>", "<ecs/QueryExpression.h>","<ecs/EventHandlerQuery.h>","<rl/events/events.h>"]]
    lines+= ["\n"]
    
    pgnlines = [];
    for e in events:
        pgnlines += generate_event_handler_query_class(e)
        
    return lines + nswrap("pgn", pgnlines)
    
def generate_event_handler_query_source():
    lines = [ "#include " + s for s in ["\"EventHandlerQueries.h\""]]
    lines+= ["\n"]
    
    sep = "//" + "".join(["-" for i in range(100)])
    
    pgnlines = [];
    for e in events:
        e_name = e[0]
        e_args = e[1]
        
        ecc = evt2camelcase(e_name)
        args = ", ".join(e_args)
        
        pgnlines += [sep,"void cEventHandlerQuery_%s::Handle( %s )"%(ecc,args),"{","\tif (!mQuery) return;","\tassert(false);","}",""]
        
    return lines + nswrap("pgn", pgnlines)
        
    
    
def generate_register_events():
    lines = [ "#include " + s for s in ["\"events.h\"", "<rl/SystemMgrRL.h>", "<ecs/EventHandlerQuery.h>","\"EventHandlerQueries.h\""]]
    
    pgnlines = [];
    for e in events:
        e_name = e[0]
        e_args = e[1]
        ecc = evt2camelcase(e_name)
        args = ", ".join(e_args)
        classname = "cEventHandlerQuery_%s"%( ecc)
        line = "mAllEventHandlerQueries[\"%s\"]= std::dynamic_pointer_cast<cEventHandlerQueryBase>(std::make_shared<%s>());"%(ecc,classname)
        pgnlines += [line]
        
    return lines + nswrap("pgn", ["void cSystemMgrRL::RegisterEventHandlers()","{"] + pad_lines(pgnlines,1) + ["}"])
    
s_evt_header = "\n".join(generate_events_header())
s_evt_source = "\n".join(generate_events_source())
s_evtreg_source = "\n".join(generate_events_reg())
#s_evt_mgr_header = "\n".join(generate_event_mgr_header())
s_register_source = "\n".join(generate_register_events())
s_ehq_header = "\n".join(generate_event_handler_query_header())
s_ehq_source = "\n".join(generate_event_handler_query_source())

write_to_file("../events/events.h",s_evt_header)
write_to_file("../events/events.cpp",s_evt_source)
write_to_file("../events/events_reg.cpp",s_evtreg_source)
#write_to_file("../events/evt_register.cpp",s_register_source)
#write_to_file("../events/EventHandlerQueries.h",s_ehq_header)
#write_to_file("../events/EventHandlerQueries.cpp",s_ehq_source)