#!/bin/bash
input_file=$1
output_file=$2
year=$3
dataset=$4
mkdir -p  "logs/${year}/${year}${dataset}"
log_file="logs/${year}/${year}${dataset}/$(basename ${input_file}).log"
failed_filelist="logs/failed_filelist_${year}${dataset}.txt"

echo "=== System Info (uname -a) ==="
uname -a
echo ""
echo "=== Running executable ==="
# Replace the path with your actual executable path
ldd /cmsuf/data/store/user/t2/users/neha.rawal/CSCAgeing_Run2_study/New_environment/CMSSW_15_0_4/src/Src/build/Analysis_exe
# Define the path to your actual processing script (this same file, or another)
##source /cvmfs/cms.cern.ch/cmsset_default.sh
##cd /cmsuf/data/store/user/t2/users/neha.rawal/CSCAgeing_Run2_study/again_env/CMSSW_13_3_0/src/
##eval `scramv1 runtime -sh`
##cd -
##echo "Processing $input_file" > $log_file
##python3 -c "import ROOT; f=ROOT.TFile.Open('${input_file}'); exit(1) if not f or f.IsZombie() else exit(0)" >> $log_file 2>&1
##if [ $? -ne 0 ]; then
##		echo "ERROR: Could not open $input_file with ROOT" >> $log_file
##		exit 1
##fi
##root -l -b -q 'check_file_open.C("'"${input_file}"'")' >> $log_file 2>&1
##if [ $? -ne 0 ]; then
##    echo "ERROR: Could not open $input_file with ROOT" >> $log_file
##    exit 1
##fi

# Run your actual analysis
/cmsuf/data/store/user/t2/users/neha.rawal/CSCAgeing_Run2_study/New_environment/CMSSW_15_0_4/src/Src/build/Analysis_exe "$input_file" "$output_file" "$year" >> $log_file 2>&1
#../../Src/build/Analysis_exe "$input_file" "$output_file" "$year" >> $log_file 2>&1
if [ $? -ne 0 ]; then
	    echo "ERROR: Analysis failed for $input_file" >> $log_file
			echo "$input_file $output_file" >> $failed_filelist
			exit 2
fi
echo "Success for $input_file" >> $log_file
exit 0
