import sys

start_t=long(sys.argv[1])*1000000+long(sys.argv[2])-50000
end_t=long(sys.argv[3])*1000000+long(sys.argv[4])+50000

for line in sys.stdin:
    sec=line.split('{')[2].split(':')[1].split(',')[0]
    usec=line.split('{')[2].split(':')[2].split('}')[0]
    curr_t=long(sec)*1000000+long(usec)
    if start_t <= curr_t and end_t >= curr_t:
       print line.rstrip('\n')
