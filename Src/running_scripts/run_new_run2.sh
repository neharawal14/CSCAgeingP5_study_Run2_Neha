#!/bin/bash
# Navigate to the directory
cd /afs/cern.ch/user/n/nrawal/work/CSCAgeingP5_Run2_code/Src/running_scripts/
# Assign the first argument passed to the script to variable 'arg1'
arg1=$1
echo " first argument"
echo $arg1
#g++ -I $ROOTSYS/include main_run2.C ../code_area/Src/pressure_dependence_removal_instlumi.C `root-config --glibs` `root-config --libs` `root-config --cflags`  -L $ROOTSYS/lib -o executable_complete

./executable_complete $arg1
#"/eos/home-n/nrawal/CSCAgeing/Run2_combine/"
#root -l -b -q 'running_produceHistos.C("", "csc_output_run2_'${arg1}'_tree_HV_updated.root", "'${arg1}'", "/afs/cern.ch/user/n/nrawal/work/CSCAgeing_code_study/applying_correction/", "/afs/cern.ch/user/n/nrawal/work/CSCAgeing_code_study/applying_correction/plotfolder/")'
