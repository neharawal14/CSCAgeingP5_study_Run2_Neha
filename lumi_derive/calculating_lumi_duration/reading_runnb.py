# File paths
input_file = "../../Src/IntegrateLumi_2018.h"  # Replace with the actual file path
output_file = "runnb_2018.txt"  # Replace with the desired output file path

# Open the input .h file for reading and output text file for writing
with open(input_file, 'r') as infile, open(output_file, 'w') as outfile:
    for line in infile:
        # Check if the line contains the pattern "if(run == <run_number>)"
        if "if(run ==" in line:
            # Split the line to extract the run number (it's the number after "run ==")
            parts = line.split("==")
            run_number = parts[1].split(")")[0].strip()  # Extract the run number and clean it
            # Write the run number to the output file
            outfile.write(f"{run_number}\n")

print("Run numbers extracted and written to output file.")

