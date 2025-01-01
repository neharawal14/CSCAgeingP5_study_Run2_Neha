#!/bin/bash
# Navigate to the directory
cd /afs/cern.ch/user/n/nrawal/work/CSCAgeing_code_study/applying_correction/
# Assign the first argument passed to the script to variable 'arg1'
arg1=$1
echo " first argument"
echo $arg1
#g++ -I $ROOTSYS/include main.C ../code_area/Src/pressure_dependence_removal_instlumi.C `root-config --glibs` `root-config --libs` `root-config --cflags`  -L $ROOTSYS/lib -o executable

./executable $arg1
