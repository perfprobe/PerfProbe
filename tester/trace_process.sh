# Pull traces from a mobile device
adb pull /sdcard/*.logcat .
adb shell ls /sdcard/*.tracedump | tr '\r' ' ' | xargs -n1 adb pull

# Collect Panappticon trace from a server and parse it
mkdir user
mkdir kernel
./parse_event.sh

# Compute delay for each run and parse function call trace
n=1; for f in $(cat traceview.info); do tid=$(echo $f | cut -d'.' -f1); c=$(cat app.latency | head -n$n | tail -n1 | cut -d',' -f1); ./dmtracedump -o $f > $tid.$c.traceview; n=$(($n+1)); done 
