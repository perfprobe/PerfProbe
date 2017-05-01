import sys

func_time={}

for line in sys.stdin:
    time=line.split(' ')[3]
    func=line.split(' ')[4].lstrip('.')
    print time, func
    if func in func_time:
       func_time[func] = func_time[func] + long(time)
    else:
       func_time[func] = long(time)
    if func_time[func] < 0:
       print func, time, long(time)

for key in func_time:
    print key, func_time[key]
