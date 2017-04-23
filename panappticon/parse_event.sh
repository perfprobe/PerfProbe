rm kernel.$1.decoded user.$1.decoded $1.decoded
for f in $(ls kernel/*);
do
    ./event-decompressor < $f > tmp.1; ./event-decoder < tmp.1 >> kernel.$1.decoded
done
rm tmp.1;
for f in $(ls user/*);
do
    ./parse < $f >> user.$1.decoded
done
cat nexus6.kernel.gallery.decoded user.$1.decoded > $1.decoded
./sort_json.sh $1.decoded
mv sorted.$1.decoded $1.decoded
rm user.$1.decoded kernel.$1.decoded
