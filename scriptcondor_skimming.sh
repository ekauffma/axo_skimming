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
echo "copied input file from $1"

./skim input.root skimmed.root
echo "skim successful"

cd axol1tl-producer
python3 axol1tl_inference.py -i ../skimmed.root -o ../ -p -v v3 v4
cd ..
echo "added axo score"

xrdcp skimmed_wScores.root root://eoscms.cern.ch//store/group/phys_exotica/axol1tl/Data_ScoutingNano_withAXOscore/2024I/$2 
echo "copied skimmed file to eos"

rm input.root
rm skimmed.root
rm skimmed_wScores.root

cd ../..
