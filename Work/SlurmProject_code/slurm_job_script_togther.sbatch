#!/bin/bash
#SBATCH --job-name=root_array
#SBATCH --output=logs/slurm-%A_%a.out
#SBATCH --error=logs/slurm-%A_%a.err
#SBATCH --time=00:20:00
#SBATCH --mem=700MB
#SBATCH --qos=avery
#SBATCH --cpus-per-task=1
#SBATCH --constraint=el9
# Get line N from filelist.txt where N = SLURM_ARRAY_TASK_ID
# Default filelist if not passed
#: "${FILELIST:=filelist_2016C.txt}"
echo "FILELIST name $FILELIST"
line=$(sed -n "$((SLURM_ARRAY_TASK_ID + 1))p" $FILELIST)
input_file=$(echo $line | cut -d' ' -f1)
output_file=$(echo $line | cut -d' ' -f2)
year=$(echo $line | cut -d' ' -f3)
dataset=$(echo $line | cut -d' ' -f4)
echo "Processing input file: $input_file"
echo "Processing output file: $output_file"
echo "Processing year : $year"
echo "Processing dataset : $dataset"
# Run your analysis
./single_file_run.sh "$input_file" "$output_file" "$year" "$dataset"
