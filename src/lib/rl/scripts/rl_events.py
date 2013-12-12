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
           
def evt2camelcase(s):
    return "".join([ s2.capitalize() for s2 in s.split()])
    
def evt2constant(s):
    return "_".join([ s2.upper() for s2 in s.split()])
    
# pad the lines for namespace purposes
def pad_lines( lines, tnum ):
    ts = "".join([ "\t" for i in range(tnum)]);
    return [ ts + s for s in lines]

def nswrap( ns, lines):
    return ["namespace " + ns, "{"] + pad_lines(lines,1) + ["}"]    
    
    
def write_to_file( fname, text):
    fp = open(fname,'w')
    fp.write(text)
    fp.close()