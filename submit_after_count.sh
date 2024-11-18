count="1"

file=$1
while read -r line; do

	

	input_filename=$(echo $line | awk '{print $1}')

	if [[ $count -ge 8900 ]]; then
		echo "Count: $count"
		echo "Input filename: $input_filename"
		sh SubmitToCondor.sh output_${1}_${count} ${input_filename}
	fi
	((count++))

done < $file

exit 0
