This repository is for submitting condor jobs to skim 2024F scouting nanoaod down to events which pass either one of the AXO scouting bits or ZeroBias scouting.
Before running, the path to the proxy and skim.cxx file in scriptcondor_skimming_template.sub must be changed as well as the output, error, and log paths in scriptcondor_skimming_template.sub.
To run, run the command "sh submit.sh input_\*.txt" where input_*.txt is whichever text file containing the input file paths you want to run over.
