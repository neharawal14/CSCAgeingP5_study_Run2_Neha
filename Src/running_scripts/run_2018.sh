#!/bin/bash
# Navigate to the directory
cd /afs/cern.ch/user/n/nrawal/work/CSCAgeingP5_Run2_code/Src/running_scripts/
# Assign the first argument passed to the script to variable 'arg1'
arg1=$1
arg2=$2
echo " first argument"
echo $arg1
#g++ -I $ROOTSYS/include main_2018.C ../code_area/Src/pressure_dependence_removal_instlumi.C `root-config --glibs` `root-config --libs` `root-config --cflags`  -L $ROOTSYS/lib -o executable_2018

./executable_2018 $arg1 $arg2
