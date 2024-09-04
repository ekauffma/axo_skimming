#!/bin/bash
/bin/pwd
/bin/uname -a
/bin/hostname

echo "listing directory contents:"
ls -lah

source /cvmfs/cms.cern.ch/cmsset_default.sh
echo $PWD
export X509_USER_PROXY=$PWD/x509_proxy

voms-proxy-info --all

cmsrel CMSSW_14_0_15

cd CMSSW_14_0_15/src
echo $PWD

cp -r ../../axol1tl-producer .
cp -r ../../run3_ugt_ml .

cd run3_ugt_ml
pip install .
cd ..
echo "Installed HLS firmware for the CMS Run 3 uGT Neural Networks"

cmsenv
scram b -j 8

echo "finished setting up cmssw"

cp ../../skim.cxx .
g++ -O3 -o skim skim.cxx $(root-config --cflags --libs)
echo "built skimming program"

xrdcp $1 input.root
echo "copied input file"

cd axol1tl-producer
python3 axol1tl_inference.py -i ../input.root -o ../ -p
cd ..
rm input.root
echo "added axo score"

./skim input_wScores.root output.root
echo "skim successful"

xrdcp output.root root://cmseos.fnal.gov//store/user/lpctrig/ekauffma/Scouting_2024F_Skims_wScores/$2
echo "copied skimmed file to eos"

rm input_wScores.root
rm output.root
