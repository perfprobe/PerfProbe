cat $1 | grep CONTEXT_SWITCH > $1.tmp
cut -d'{' -f3 $1.tmp | cut -d':' -f2 | cut -d',' -f1 > $1.tmp.1
cut -d'{' -f3 $1.tmp | cut -d':' -f3 | cut -d'}' -f1 > $1.tmp.2
cut -d'{' -f4 $1.tmp | cut -d':' -f2 | cut -d',' -f1 > $1.tmp.3
cut -d'{' -f4 $1.tmp | cut -d':' -f3 | cut -d',' -f1 > $1.tmp.4
cut -d'{' -f4 $1.tmp | cut -d':' -f4 | cut -d'}' -f1 | cut -d'"' -f2 > $1.tmp.5
paste -d',' $1.tmp.1 $1.tmp.2 $1.tmp.3 $1.tmp.4 $1.tmp.5 > $1.cpu
cat $1 | grep SOCK > $1.tmp
cut -d'"' -f4 $1.tmp > $1.tmp.1
cut -d'{' -f3 $1.tmp | cut -d':' -f2 | cut -d',' -f1 > $1.tmp.2
cut -d'{' -f3 $1.tmp | cut -d':' -f3 | cut -d'}' -f1 > $1.tmp.3
paste -d',' $1.tmp.1 $1.tmp.2 $1.tmp.3 > $1.sock
cat $1 | grep IO > $1.tmp
cut -d'"' -f4 $1.tmp > $1.tmp.1
cut -d'{' -f3 $1.tmp | cut -d':' -f2 | cut -d',' -f1 > $1.tmp.2
cut -d'{' -f3 $1.tmp | cut -d':' -f3 | cut -d'}' -f1 > $1.tmp.3
paste -d',' $1.tmp.1 $1.tmp.2 $1.tmp.3 > $1.disk
rm $1.tmp*
