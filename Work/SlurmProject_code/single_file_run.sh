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
ldd  /cmsuf/data/store/user/t2/users/neha.rawal/CSCAgeing_Run2_study/CMSSW_13_3_0/src/CSCAgeingP5_study_Run2_Neha/Src/build/Analysis_exe
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

# print whether cvmfs mount or not. because the jobs are failing
echo "cvmfs mount check"
ls "/cvmfs/"
echo "cvmfs cms mount check"
ls "/cvmfs/cms.cern.ch/"
echo "cvmfs cms mount -el8  check"
ls "/cvmfs/cms.cern.ch/el8_amd64_gcc12/"
echo "cvmfs cms mount -el8 lcg  check"
ls "/cvmfs/cms.cern.ch/el8_amd64_gcc12/lcg/"
echo "cvmfs cms mount -el8 lcg root  check"
ls "/cvmfs/cms.cern.ch/el8_amd64_gcc12/lcg/root/"
# Run your actual analysis
/cmsuf/data/store/user/t2/users/neha.rawal/CSCAgeing_Run2_study/CMSSW_13_3_0/src/CSCAgeingP5_study_Run2_Neha/Src/build/Analysis_exe "$input_file" "$output_file" "$year" >> $log_file 2>&1
#../../Src/build/Analysis_exe "$input_file" "$output_file" "$year" >> $log_file 2>&1
if [ $? -ne 0 ]; then
	    echo "ERROR: Analysis failed for $input_file" >> $log_file
			echo "$input_file $output_file" >> $failed_filelist
			exit 2
fi
echo "Success for $input_file" >> $log_file
exit 0
