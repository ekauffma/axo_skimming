#!/bin/bash

# Define the path pattern for the files to search
file_pattern="output_input_files_2024G.txt_*/log/scriptcondor_0.log"

# Initialize counters
file_count=0
remove_count=0

# Loop over each file that matches the pattern
for file in $file_pattern; do
  # Check if the file exists (in case no files match the pattern)
  if [[ -f "$file" ]]; then
    ((file_count++))
    # Use tail to read the last 50 lines (or adjust this as needed) and search for SYSTEM_PERIODIC_REMOVE
    if tail -n 5 "$file" | grep -q "SYSTEM_PERIODIC_REMOVE"; then
      echo "SYSTEM_PERIODIC_REMOVE found in $file"
      ((remove_count++))
    fi
  else
    echo "No files found matching the pattern: $file"
  fi
done

# Print the final counts
echo "Total files checked: $file_count"
echo "Total occurrences of SYSTEM_PERIODIC_REMOVE: $remove_count"
