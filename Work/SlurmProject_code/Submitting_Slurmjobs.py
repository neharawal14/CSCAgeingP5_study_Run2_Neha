import os
import argparse

parser = argparse.ArgumentParser(description="Prepare file list for SLURM job array.")
parser.add_argument('--input_path', required=True, help='Top-level input directory containing ROOT files')
parser.add_argument('--output_path', required=True, help='Output directory for processed files')
parser.add_argument('--year', required=True, help='Year (e.g., 2016)')
parser.add_argument('--dataset', required=True, help='Dataset name (e.g., B)')
args = parser.parse_args()

input_path = args.input_path
output_path = args.output_path
year = args.year
dataset_name = args.dataset

os.makedirs("logs", exist_ok=True)
os.makedirs("slurm_scripts", exist_ok=True)

filelist_path = f"filelist_{year}{dataset_name}.txt"

with open(filelist_path, "w") as filelist:
    for dirpath, dirnames, filenames in os.walk(input_path):
        for filename in filenames:
            if filename.endswith(".root"):
                input_file = os.path.join(dirpath, filename)

                # Optional: preserve subdir structure under output path
                output_file = os.path.join(output_path, filename)

                # Ensure parent directory of output path exists
                os.makedirs(os.path.dirname(output_file), exist_ok=True)

                # Write input and output file path to list
                filelist.write(f"{input_file} {output_file}\n")

print(f"Saved list of ROOT files to: {filelist_path}")
with open(f"filelist_{year}{dataset_name}.txt") as f:
    n_lines = sum(1 for _ in f)
os.system(f"sbatch --array=0-{n_lines - 1} --export=ALL,FILELIST=filelist_{year}{dataset_name}.txt slurm_job_script_togther.sbatch")
