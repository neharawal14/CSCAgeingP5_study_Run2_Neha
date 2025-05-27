#!/bin/bash
#SBATCH --job-name=root_array
#SBATCH --output=logs/slurm-%A_%a.out
#SBATCH --error=logs/slurm-%A_%a.err
#SBATCH --time=00:15:00
#SBATCH --mem=200MB
#SBATCH --qos=avery
#SBATCH --cpus-per-task=1
# Get line N from filelist.txt where N = SLURM_ARRAY_TASK_ID
# Default filelist if not passed
#: "${FILELIST:=filelist_2016C.txt}"
echo "FILELIST name $FILELIST"
line=$(sed -n "$((SLURM_ARRAY_TASK_ID + 1))p" $FILELIST)
input_file=$(echo $line | cut -d' ' -f1)
output_file=$(echo $line | cut -d' ' -f2)

echo "Processing file: $input_file_$output_file"

# Run your analysis
./single_file_run.sh "$input_file" "$output_file" "2016"
