# K, app_name
app=$2
rm method_feature.meta method_feature.val
for i in $(cut -d',' -f1 $app.latency);
do
    if [ ! -e methods.inner.$i ]; then continue; fi
    cat methods.inner.$i | python MethodFeature.py > method.feature.$i
    for f in $(cat method.feature.$i | sort -nr -k2 | head -n$1 | cut -d' ' -f1);
    do
        echo $f >> method_feature.meta
    done
done
sort method_feature.meta | uniq > method_feature.meta.tmp
mv method_feature.meta.tmp method_feature.meta
for i in $(cut -d',' -f1 $app.latency);
do
    if [ ! -e methods.inner.$i ]; then continue; fi
    for line in $(cat method_feature.meta);
    do
       if [ $(cat method.feature.$i | grep "$line " | wc -l) -eq 0 ]; then
           echo -n ",0" >> method_feature.val
       else
           echo -n ",$(cat method.feature.$i | grep "$line " | cut -d' ' -f2)" >> method_feature.val
       fi
    done
    echo "" >> method_feature.val
done
