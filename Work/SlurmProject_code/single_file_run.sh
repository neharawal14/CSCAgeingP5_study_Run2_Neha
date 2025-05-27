#!/bin/bash
input_file=$1
output_file=$2
year=$3

log_file="logs/${year}/$(basename ${input_file}).log"

##echo "Processing $input_file" > $log_file
python3 -c "import ROOT; f=ROOT.TFile.Open('${input_file}'); exit(1) if not f or f.IsZombie() else exit(0)" >> $log_file 2>&1
if [ $? -ne 0 ]; then
		echo "ERROR: Could not open $input_file with ROOT" >> $log_file
		exit 1
fi
##root -l -b -q 'check_file_open.C("'"${input_file}"'")' >> $log_file 2>&1
##if [ $? -ne 0 ]; then
##    echo "ERROR: Could not open $input_file with ROOT" >> $log_file
##    exit 1
##fi

# Run your actual analysis
/cmsuf/data/store/user/t2/users/neha.rawal/CSCAgeing_Run2_study/CMSSW_13_3_0/src/CSCAgeingP5_study_Run2_Neha/Src/build/Analysis_exe "$input_file" "$output_file" "$year" >> $log_file 2>&1
#../../Src/build/Analysis_exe "$input_file" "$output_file" "$year" >> $log_file 2>&1
if [ $? -ne 0 ]; then
    echo "ERROR: Analysis failed for $input_file" >> $log_file
    exit 2
fi

echo "Success for $input_file" >> $log_file
exit 0
