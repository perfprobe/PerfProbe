# test_times, process_name
for ((i=1;i<$1;i=i+1));
do
   tid=$(adb shell ps | grep $2 | cut -c11- | cut -d' ' -f1);
   if [ "$tid" != "" ]; then
      tt=$(date +%s);
      adb shell am profile start $tid 10000 /sdcard/$tid.$tt.tracedump
      echo "Trace $i $tid"
   fi
   echo "Before test $i"
   monkeyrunner test_1.py
   echo "End test $i"
   if [ "$tid" != "" ]; then
      adb shell am profile stop $tid
      echo "Dump $i"
   fi
   monkeyrunner test_2.py
   sleep 25
done
