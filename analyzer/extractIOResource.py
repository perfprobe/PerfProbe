mport sys

total=0
lock_state=0

prev_sec=0
prev_usec=0

tid=int(sys.argv[1])

for line in sys.stdin:
    event=line.split(',')[0]
    sec=long(line.split(',')[1])
    usec=long(line.split(',')[2])
    if (sec-prev_sec)*1000000+(usec-prev_usec) < 0:
       lock_state=0
    if "_BLOCK" in event or "_WAIT" in event:
       if lock_state == 0:
          prev_sec=sec
          prev_usec=usec
          lock_state=1
          notify_state=0
    if "_RESUME" in event or "_WAKE" in event:
       if lock_state == 0:
          continue
       elif "_RESUME" in event or notify_state == 1:
          total=total+(sec-prev_sec)*1000000+(usec-prev_usec)
       lock_state=0
    if "_NOTIFY" in event:
       if lock_state == 1:
           notify_state=1

print tid, total
