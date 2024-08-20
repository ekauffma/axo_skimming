#!/bin/bash
/bin/pwd
/bin/uname -a
/bin/hostname

source /cvmfs/cms.cern.ch/cmsset_default.sh
echo $PWD
export X509_USER_PROXY=$PWD/x509_proxy

voms-proxy-info --all

cmsrel CMSSW_14_0_0_pre2

cd CMSSW_14_0_0_pre2/src
echo $PWD

cmsenv
scram b -j 8

echo "finished setting up cmssw"

cp ../../skim.cxx .
g++ -O3 -o skim skim.cxx $(root-config --cflags --libs)
echo "built skimming program"

xrdcp $1 input.root
echo "copied input file"

./skim input.root output.root
echo "skim successful"

xrdcp output.root root://cmseos.fnal.gov//store/user/lpctrig/ekauffma/Scouting_2024F_Skims/$2

echo "copied skimmed file to eos"

rm input.root
rm output.root
