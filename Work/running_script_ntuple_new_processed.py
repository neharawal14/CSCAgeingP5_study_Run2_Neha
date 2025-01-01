import os
import subprocess
import ROOT
import argparse

# Argument parsing for inputs
parser = argparse.ArgumentParser(description='Process some inputs.')
parser.add_argument('--dataset_name', type=str, required=True, help='The name of the dataset, e.g., "2016B"')
parser.add_argument('--year', type=str, required=True, help='Year of the dataset, e.g., "2016B"')
parser.add_argument('--input_path', type=str, required=True, help='The path to the input file, e.g. "/path/to/file"')
parser.add_argument('--output_path', type=str, required=True, help='Output file path ')
args = parser.parse_args()

dataset_name = args.dataset_name
input_path = args.input_path
output_path = args.output_path
year = args.year

# Log files for processed files and errors
processed_files_log = f"processed_files_{year}{dataset_name}.log"
log_file = open(f"log_{year}.txt", "a")
print(" started this program")
# Load previously processed files into a set for quick lookup
processed_files = set()
if os.path.exists(processed_files_log):
    with open(processed_files_log, "r") as f:
        processed_files = set(line.strip() for line in f)

# Iterate over all the directories and files
for dirpath, dirnames, filenames in os.walk(input_path):
    for filename in filenames:
        if filename.endswith(".root"):
            file_name = os.path.basename(filename)
            print(" file name ", file_name)
            input_file_path = os.path.join(dirpath, file_name)
            print(" input file path ", input_file_path)
            final_path = os.path.join(output_path, file_name)

            # Check if file has already been processed
            if os.path.exists(processed_files_log):
                if file_name in processed_files:
                 print(f"Skipping already processed file: {file_name}")
                 continue

            print(f"Processing file: {file_name}")
            try:
                f = ROOT.TFile.Open(input_file_path)
            except IOError:
                log_file.write(f"Failed to open file: {input_file_path}\n")
            else:
                print(f"Opened file: {input_file_path}")
                process_string = f"./single_file_run.sh {input_file_path} {final_path} {year}"
                print(f"Process string: {process_string}")
                log_file.write(f"Process string: {process_string}\n")
                
                # Run the processing script
                subprocess.call(process_string, shell=True)

                # After successful processing, record the file as processed
                with open(processed_files_log, "a") as processed_log:
                    processed_log.write(file_name + "\n")
                processed_files.add(file_name)  # Add to in-memory set for this run

log_file.close()
