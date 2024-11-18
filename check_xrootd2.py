import subprocess
import os

def sizeof_fmt(num, suffix="B"):
    for unit in ("", "Ki", "Mi", "Gi", "Ti", "Pi", "Ei", "Zi"):
        if abs(num) < 1024.0:
            return f"{num:3.1f}{unit}{suffix}"
        num /= 1024.0
    return f"{num:.1f}Yi{suffix}"

# Define server and directory path
#xrootd_server = "root://hip-cms-se.csc.fi"
#directory_path = "/store/user/pinkaew/scouting_nano_prod_v0p1_golden/"
xrootd_server = "root://eoscms.cern.ch"

# MC 
out_filename = 'input_files_MC.json'
sample_names = ["DYto2L_M-50_TuneCP5_13p6TeV_pythia8",
                "TTto2L2Nu_TuneCP5_13p6TeV_powheg-pythia8",
                "JPsiToMuMu_PT-0to100_pythia8-gun"]
path = "//store/group/phys_exotica/axol1tl/Data_ScoutingNano_withAXOscore/"

# Data 
# out_filename = 'input_files_data.json'
# sample_names = ["2024F","2024G"]
# path = "//store/group/phys_exotica/axol1tl/Data_ScoutingNano_withAXOscore/"

try:
    os.remove(out_filename)
except OSError:
    pass

with open(out_filename, 'a') as out_file:
    out_file.write("{ \n")
    for sample_name in sample_names:                
        directory_path = path+sample_name+"/"
        # Run the xrdfs command to list files
        command = f"xrdfs {xrootd_server} ls -l -R {directory_path}"
        result = subprocess.run(command, shell=True, capture_output=True, text=True)

        # Parse the output
        total_size = 0
        total_number = 0

        out_file.write("\t\""+sample_name+"\" : { \n")
        out_file.write("\t\t\"files\" : { \n")
        for line in result.stdout.splitlines():
            parts = line.split()
            #print(parts)
            if len(parts) > 4 and parts[0] != 'd' and parts[-1][-5:]==".root":  # Check if it's a file line and not a directory line
                out_file.write("\""+xrootd_server+parts[-1]+" : \"Events\",\n")
                total_size += int(parts[3])
                total_number += 1
        out_file.write("\t\t} \n")
        out_file.write("\t}, \n")

        # Print the total size
        print(sample_name+"---------------")
        print(f"Total size: {total_size} bytes")
        print(sizeof_fmt(total_size))
        print("Total number of files:")
        print(total_number)

    out_file.write("} \n")

