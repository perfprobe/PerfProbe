# Extract method feature from function call trace
for f in $(ls *.traceview); do tid=$(echo $f | cut -d'.' -f1); i=$(echo $f | cut -d'.' -f2); ./extract_inner_method.sh $i $tid; done
for f in $(ls *.traceview); do tid=$(echo $f | cut -d'.' -f1); i=$(echo $f | cut -d'.' -f2); ./extract_method.sh $i $tid; done 

# Prune to get top-K methods for DT selection
./get_topk_method.sh $app

# DT selection on function features
