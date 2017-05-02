# Extract method feature from function call trace
for f in $(ls *.traceview); do tid=$(echo $f | cut -d'.' -f1); i=$(echo $f | cut -d'.' -f2); ./extract_inner_method.sh $i $tid; done
for f in $(ls *.traceview); do tid=$(echo $f | cut -d'.' -f1); i=$(echo $f | cut -d'.' -f2); ./extract_method.sh $i $tid; done 

# Prune to get top-K methods for DT selection
./get_topk_method.sh $app

# DT selection on function features
echo -n "label" > feature.meta; for l in $(cat method_feature.meta); do echo -n ",$l" >> feature.meta; done
for l in $(cut -d',' -f2 *.latency); do if [ $l -gt $thres ]; then echo "-1" >> tmp; else echo "+1" >> tmp; fi; done
paste tmp method_feature.val > dtree.csv
sed -i 's/\t//g' dtree.csv; rm tmp
python dtree.py dtree.csv
