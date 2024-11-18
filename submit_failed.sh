#!/bin/bash

count="1"

file=$1
while read -r line; do

	#echo count $count

	input_filename=$(echo $line | awk '{print $1}')

	#echo $input_filename

	logfile="output_input_files_2024G.txt_"$count$"/log/scriptcondor_0.log"


	if [[ -f "$logfile" ]]; then
		# Use tail to read the last 50 lines (or adjust this as needed) and search for SYSTEM_PERIODIC_REMOVE
	    if tail -n 5 "$logfile" | grep -q "SYSTEM_PERIODIC_REMOVE"; then
	    	echo "SYSTEM_PERIODIC_REMOVE found in $logfile"
			echo Resubmitting $count
			#sh SubmitToCondor.sh output_testll_${1}_${count} ${input_filename}
		fi
	fi

	count=$[$count+1]

done < $file

#exit 0
