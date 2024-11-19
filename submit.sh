count="1"
file=$1
while read -r line; do

echo count $count

input_filename=$(echo $line | awk '{print $1}')

echo $input_filename

if [ ! -f /eos/cms/store/group/phys_exotica/axol1tl/Data_ScoutingNano_withAXOscore/2024I/output_${1}_${count}.root ]; then
	echo "File not found.\n"
	sh SubmitToCondor.sh output_${1}_${count} ${input_filename}
else
	echo "File exists. Will not submit job."
fi

count=$[$count+1]

done < $file

exit 0
