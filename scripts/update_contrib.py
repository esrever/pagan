import sys
import os
import time
import datetime
import calendar
import shutil

days_diff = float(sys.argv[2])
start_folder = sys.argv[1]
out_folder = sys.argv[3]

testOnly = len(sys.argv) > 4;

for root, dirs, files in os.walk( start_folder ):
    for basename in files:
        filename = os.path.join(root, basename)
        
        tfile = os.path.getmtime(filename);
        tnow = calendar.timegm(time.gmtime())
        tdiff = abs(tfile - tnow);
        day_diff = tdiff/(60*60*24)
        if(day_diff < days_diff):
            (fdir, fname) = os.path.split(filename)
            fdir_rel = os.path.relpath(fdir,start_folder)
            fdir_out = os.path.join(out_folder, fdir_rel);
            fname_out = os.path.join(fdir_out, fname);
            if not os.path.exists(fdir_out):
                os.makedirs(fdir_out)
            if not testOnly:
                shutil.copyfile(filename, fname_out)
            else:
                print fname_out
            