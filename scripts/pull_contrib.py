import sys
import os
import time
import datetime
import calendar
import shutil

out_folder = sys.argv[1]
in_folder = sys.argv[2]

testOnly = len(sys.argv) > 3;

for root, dirs, files in os.walk( in_folder ):
    for basename in files:
        filename = os.path.join(root, basename)
        
        (fdir, fname) = os.path.split(filename)
        fdir_rel = os.path.relpath(fdir,in_folder)
        fdir_out = os.path.join(out_folder, fdir_rel);
        fname_out = os.path.join(fdir_out, fname);
        if not os.path.exists(fdir_out):
            os.makedirs(fdir_out)
        if not testOnly:
            shutil.copyfile(filename, fname_out)
        else:
            print fname_out
            