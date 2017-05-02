# K, app_name
rm method_all_feature.meta method_all_feature.val
for i in $(cut -d',' -f1 $app.latency);
do
    if [ ! -e methods.all.$i ]; then continue; fi
    cat methods.all.$i | python MethodFeature.py > method_all.feature.$i
    for f in $(cat method_all.feature.$i | sort -nr -k2 | head -n$1 | cut -d' ' -f1);
    do
        echo $f >> method_all_feature.meta
    done
done
sort method_all_feature.meta | uniq > method_all_feature.meta.tmp
mv method_all_feature.meta.tmp method_all_feature.meta
for i in $(cut -d',' -f1 $app.latency);
do
    if [ ! -e methods.all.$i ]; then continue; fi
    for line in $(cat method_all_feature.meta);
    do
       if [ $(cat method_all.feature.$i | grep "$line " | wc -l) -eq 0 ]; then
           echo -n ",0" >> method_all_feature.val
       else
           echo -n ",$(cat method_all.feature.$i | grep "$line " | cut -d' ' -f2)" >> method_all_feature.val
       fi
    done
    echo "" >> method_all_feature.val
done
