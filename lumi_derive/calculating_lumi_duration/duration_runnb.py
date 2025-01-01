import csv
import os
from datetime import datetime

debug = False

def split_csv(path_csv, path_run_list):
    f = open(path_run_list)
    print(path_csv)
    print("file opening", path_run_list)
    output_string = "./duration_2018_goldenjson.txt"
    output_file = open(output_string, "w")
    output_file.write("run_nb\tstart_time\tend_time\n")
    
    for line in f:
        run_nb = line.strip()
        print(run_nb)
        file_csv_path = path_csv + run_nb + ".csv"
        print("csv path to read", file_csv_path)

        if os.path.isfile(file_csv_path):
            with open(file_csv_path, "r") as file:
                lines = file.readlines()

                # Skip the run_nb if the CSV file has fewer than two lines
                if len(lines) < 2:
                    print(f"Skipping run_nb {run_nb} due to insufficient data (less than 2 lines).")
                    continue

                # Process the first line (start time)
                first_line = lines[0]
                date_time_string = first_line.split(",")
                date_time = date_time_string[2]
                datetime_format = datetime.strptime(date_time, '%m/%d/%y %H:%M:%S')
                start_time = datetime.strftime(datetime_format, '%d-%b-%y %I.%M.%S.%f %p').upper()

                # Process the last line (end time)
                last_line = lines[-1]
                date_time_end_string = last_line.split(",")
                date_time_end = date_time_end_string[2]
                datetime_end_format = datetime.strptime(date_time_end, '%m/%d/%y %H:%M:%S')
                end_time = datetime.strftime(datetime_end_format, '%d-%b-%y %I.%M.%S.%f %p').upper()

                if debug:
                    print("run nb:", run_nb)
                    print("start time:", start_time)
                    print("end time:", end_time)

                string_in_text = f"{run_nb}\t{start_time}\t{end_time}\n"
                output_file.write(string_in_text)
        else:
            print(f"File does not exist: {run_nb}")
    
    # Close files
    f.close()
    output_file.close()

if __name__ == '__main__':
    path_csv = "../../files_HVandLumi/InstLumiPerRun/2018/"
    path_run_list = "./runnb_2018.txt"
    split_csv(path_csv, path_run_list)

