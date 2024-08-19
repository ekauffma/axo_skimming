import subprocess

def sizeof_fmt(num, suffix="B"):
    for unit in ("", "Ki", "Mi", "Gi", "Ti", "Pi", "Ei", "Zi"):
        if abs(num) < 1024.0:
            return f"{num:3.1f}{unit}{suffix}"
        num /= 1024.0
    return f"{num:.1f}Yi{suffix}"

# Define server and directory path
xrootd_server = "root://hip-cms-se.csc.fi"
directory_path = "/store/user/pinkaew/scouting_nano_prod_v0p1_golden/"

# Run the xrdfs command to list files
command = f"xrdfs {xrootd_server} ls -l -R {directory_path}"
result = subprocess.run(command, shell=True, capture_output=True, text=True)

# Parse the output
total_size = 0
total_number = 0

with open('input_files_2024F.txt', 'a') as out_file:
    for line in result.stdout.splitlines():
        parts = line.split()
        if len(parts) > 4 and parts[0] != 'd' and parts[-1][-5:]==".root":  # Check if it's a file line and not a directory line
            if "2024F" in parts[-1]: # Check if run is 2024F so that we have zerobias scouting
                out_file.write("root://hip-cms-se.csc.fi://"+parts[-1]+"\n")
                total_size += int(parts[3])
                total_number += 1

# Print the total size
print(f"Total size: {total_size} bytes")
print(sizeof_fmt(total_size))
print("Total number of files:")
print(total_number)
