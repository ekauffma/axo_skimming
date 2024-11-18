# axo_skimming
This code skims CMS Scouting NanoAODs down to smaller filesizes via a condor batch system. The code in `skim.cxx` filters for events which pass the AXOL1TL or ZeroBias HLT triggers, it also removes unused branches such as the ScoutingPFCandidates and ScoutingTracks.

## Setup instructions
This setup can be run from any HTCondor cluster, e.g. on lxplus. 

To setup the environment use CMSSW_14_0_15. 
```
ssh YOUR_USERNAME@lxplus.cern.ch 
source /cvmfs/cms.cern.ch/cmsset_default.sh
cmsrel CMSSW_14_0_15
cd CMSSW_14_0_15/src/
cmsenv
git clone git@github.com:ekauffma/axo_skimming.git
cd axo_skimming
git clone https://gitlab.cern.ch/cms-l1-ad/axol1tl-producer.git
git clone https://gitlab.cern.ch/ssummers/run3_ugt_ml
cd run3_ugt_ml 
pip3 install . 
cd ..
voms-proxy-init -voms cms
cp /tmp/x509up_u72489 x509_proxy

```

Before running, you *must* change the output, error, log and transfer input file paths in `scriptcondor_skimming_template.sub`.
You *must* also change the output directory in `scriptcondor_skimming.sh`.

### Running a local test
To run a local test run `. scriptcondor_skimming.sh root://hip-cms-se.csc.fi:///store/user/pinkaew/scouting_nano_prod_v0p1_golden/ScoutingPFRun3/crab_ScoutingPFRun3_Run2024F-v1_381946-382300_Golden/240729_151406/0000/scouting_nano_10.root test.root`. This should copy the output to the directory defined in `scriptcondor_skimming.sh`.

### Running a test with a single file 
To run a test on condor with a single file run `sh submit.sh input_onefile.txt`. 

Once the indiviudal test has finished (check status with `condor_q`) you can look at the output directory and check that that logs look okay. 

You can also check that the output root file was saved correctly in the location you specified in `scriptcondor_skimming.sh`, e.g. `ls /eos/cms/store/group/phys_exotica/axol1tl/your_output_directory/`


### Submitting a large set of jobs
Once the individual test has passed, you can run on a larger set of files with `sh submit.sh input_\*.txt` where `input\*.txt` contains a list of files you want to run over. 

The `input_\*.txt` file can be generated by editing `check_xrootd.py` and changing the directory path to the dataset and eras you want to run on. Then run `python3 check_xrootd.py` This will save a new set of input files for you. 

To check that your jobs have completed successfully you can run `. examine_condor_dirs.sh`. 

After your jobs have completed, you can produce a json file for coffea-casa by editing and running `python3 make_coffea_input_json.py`. This can then be copied to coffea-casa for running on your new files. 

## Troubleshooting
Check the status of you jobs with `condor_q`

If your jobs are being held it is possible your setup is wrong, check with `condor_q -hold`

Delete jobs with `condor_rm id_number_of_your_job`. The ID number can be found when you run `condor_q`.

More information on the lxplus condor system is here: https://batchdocs.web.cern.ch



