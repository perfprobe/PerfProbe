# Pull traces from a mobile device
adb pull /sdcard/*.logcat .
adb shell ls /sdcard/*.tracedump | tr '\r' ' ' | xargs -n1 adb pull
