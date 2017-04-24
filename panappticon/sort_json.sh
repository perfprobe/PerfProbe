cut -d':' -f4 $1 | cut -d',' -f1 > tmp.1
cut -d':' -f5 $1 | cut -d'}' -f1 > tmp.2
awk '{printf "%d\t%s\n", NR, $0}' < $1 > tmp.3;
paste tmp.1 tmp.2 tmp.3 > tmp;
sed -i 's/\t/ /g' tmp;
sort -n -k 1,1 -k 2,2 -k 3,3 tmp | cut -d' ' -f4- > sorted.$1;
rm tmp tmp.1 tmp.2 tmp.3;
