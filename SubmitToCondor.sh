#!/bin/bash
if [ -d $1 ]
then
    echo "Folder exists, exiting"
else
    mkdir $1
    mkdir $1/log
    cp scriptcondor_skimming_template.sub $1/scriptcondor.sub
    cp scriptcondor_skimming.sh $1/.
    cd $1
    sed -ie "s#OUTPUTDIR#$1#g" scriptcondor.sub
    sed -ie "s#OUTPUTFILENAME#$1.root#g" scriptcondor.sub
    sed -ie "s#FILENAMES#$2#g" scriptcondor.sub

    condor_submit scriptcondor.sub
fi
