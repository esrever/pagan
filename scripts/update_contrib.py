import sys
import os
import time
import datetime
import calendar

days_diff = int(sys.argv[2])

for root, dirs, files in os.walk( sys.argv[1] ):
    for basename in files:
        filename = os.path.join(root, basename)
        
        tfile = os.path.getmtime(filename);
        tnow = calendar.timegm(time.gmtime())
        
        tdiff = abs(tfile - tnow);
        
        day_diff = tdiff/(60*60*24)
        if(day_diff < days_diff):
            print filename