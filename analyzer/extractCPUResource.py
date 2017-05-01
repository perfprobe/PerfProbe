import sys

prev_sec=0
prev_usec=0
start_sec=0

cpu_run_sec=0
cpu_run_usec=0
cpu_sleep_sec=0
cpu_sleep_usec=0
cpu_wait_sec=0
cpu_wait_usec=0
cpu_usleep_sec=0
cpu_usleep_usec=0

cpu_wait_time=0
cpu_run_time=0
cpu_run_cycle=0
cpu_sleep_time=0
cpu_usleep_time=0

tid=int(sys.argv[1])

for line in sys.stdin:
    line=line.rstrip('\n')
    sec=long(line.split(',')[0])
    usec=long(line.split(',')[1])
    old=long(line.split(',')[2])
    new=long(line.split(',')[3])
    state=line.split(',')[4]
    freq=long(line.split(',')[5])
    if new == tid:
       if cpu_sleep_sec > 0:
          cpu_sleep_time = cpu_sleep_time + (sec-cpu_sleep_sec) * 1000000 + (usec-cpu_sleep_usec)
       elif cpu_wait_sec > 0:
          cpu_wait_time = cpu_wait_time + (sec-cpu_wait_sec) * 1000000 + (usec-cpu_wait_usec)
       elif cpu_usleep_sec > 0:
          cpu_usleep_time = cpu_usleep_time + (sec-cpu_usleep_sec) * 1000000 + (usec-cpu_usleep_usec)
       cpu_run_sec=sec
       cpu_run_usec=usec
       cpu_run_freq=freq
    elif old == tid:
       if cpu_run_sec > 0:
           cpu_run_time = cpu_run_time + (sec-cpu_run_sec) * 1000000 + (usec-cpu_run_usec)
           cpu_run_cycle = cpu_run_cycle + ((sec-cpu_run_sec) * 1000000 + (usec-cpu_run_usec)) * freq / 1000
       if "R" in state:
           cpu_wait_sec=sec
           cpu_wait_usec=usec
           cpu_sleep_sec=0
           cpu_sleep_usec=0
           cpu_usleep_sec=0
           cpu_usleep_usec=0
       elif "I" in state:
           cpu_sleep_sec=sec
           cpu_sleep_usec=usec
           cpu_wait_sec=0
           cpu_wait_usec=0
           cpu_usleep_sec=0
           cpu_usleep_usec=0
       elif "U" in state:
           cpu_usleep_sec=sec
           cpu_usleep_usec=usec
           cpu_wait_sec=0
           cpu_wait_usec=0
           cpu_sleep_sec=0
           cpu_sleep_usec=0
    prev_sec=sec
    prev_usec=usec

print tid, cpu_run_time, cpu_run_cycle / cpu_run_time, cpu_wait_time, cpu_sleep_time, cpu_usleep_time
