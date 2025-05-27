import os
import glob

year= "2016"
#dataset_list = ["B","C","D","E","F_1"]
dataset_list = ["A"]
for dataset in dataset_list:
        log_dir = f"logs/2016/"
        #print("log dir ", log_dir)
        error_logs = []
        incomplete_logs = []
        success_logs = []
        for log_file in glob.glob(os.path.join(log_dir, "*.log")):
            #print("processeing log  files:")
            with open(log_file, "r") as f:
                content = f.read()
                if "ERROR" in content:
                    error_logs.append(log_file)
                elif "Success" not in content:
                    incomplete_logs.append(log_file)
                else:
                    success_logs.append(log_file)
        print("✅ Successfully processed files:", len(success_logs))
        print("❌ Logs with ERROR:", len(error_logs))
        for log in error_logs:
            print("  -", log)
        print("⚠️ Logs with NO 'Success' line (possibly crashed):", len(incomplete_logs))
        for log in incomplete_logs:
            print("  -", log)
        # Optional: write lists to files
        with open(f"failed_logs_{year}{dataset}.txt", "w") as f:
            for log in error_logs + incomplete_logs:
                f.write(log + "\n")
