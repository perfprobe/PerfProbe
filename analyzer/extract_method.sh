tid=$2
rm methods.all.$1
c1=$(grep -n "PerformClick.run" $tid.$1.*traceview | head -n1 | cut -d':' -f1)
c2=$(grep -n "performTraversals" $tid.$1.*traceview | tail -n1 | cut -d':' -f1)
for f in $(cat $tid.$1.*traceview | head -n$c2 | tail -n$(($c2-$c1+1)) | sed -e 's/^ *//g;s/ *$//g' | grep " xit " | cut -d' ' -f1 | sort | uniq);
do
   if [ $f -lt 10 ]; then
      cat $tid.$1.*traceview | head -n$c2 | tail -n$(($c2-$c1+1)) | grep " $f ent\| $f xit" > $f.$1.out
   else
      cat $tid.$1.*traceview | head -n$c2 | tail -n$(($c2-$c1+1)) | grep "$f ent\|$f xit" > $f.$1.out
   fi
done
for f in $(ls *.$1.out);
do
    cat $f | python allMethod.py >> methods.all.$1
done
