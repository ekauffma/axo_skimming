#!/bin/bash

cd /uscms/home/ekauffma/CMSSW_14_0_0_pre2/src/
cmsenv
cd /uscms/home/ekauffma/CMSSW_14_0_0_pre2/src/axo/axo_skimming


./skim $1 $2
