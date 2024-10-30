#! /bin/bash

count="1"

#file=$1
while read -r line; do

printf $count

dirname=$(echo $line | awk '{print $1}')

printf $dirname

a=($(wc $dirname/log/scriptcondor_0.err))
lines=${a[0]}
if [ $lines -ne 1 ]; then
echo "Issue with condor job ${dirname}"
cat $dirname/log/scriptcondor_0.err
#else
#rm -rf $dirname
fi

count=$[$count+1]

done

exit 0

