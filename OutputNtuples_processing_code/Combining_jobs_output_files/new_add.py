#!/usr/bin/env python3
import os
import glob
import subprocess
import tempfile
import time

# ========================
# Config (same as original)
# ========================
dataset_list = ["D_2", "E_1", "F_1", "F_2","F_3", "G_1", "G_2", "H_1", "H_2"]  # Add more as needed
year = "2016"
chamber_list = [
    "ME11a", "ME11b", "ME12HV1", "ME12HV2", "ME12HV3", "ME13HV1", "ME13HV2", "ME13HV3",
    "ME21HV1", "ME21HV2", "ME21HV3", "ME22HV1", "ME22HV2", "ME22HV3", "ME22HV4", "ME22HV5",
    "ME31HV1", "ME31HV2", "ME31HV3", "ME32HV1", "ME32HV2", "ME32HV3", "ME32HV4", "ME32HV5",
    "ME41HV1", "ME41HV2", "ME41HV3", "ME42HV1", "ME42HV2", "ME42HV3", "ME42HV4", "ME42HV5"
]

# Optional: limit the number of concurrent jobs
MAX_PARALLEL_JOBS = 8

# NEW: summary file to quickly see which hadds failed
SUMMARY_PATH = f"hadd_summary_{year}.txt"
summary_lines = []

# List of tuples: (Popen, label, log_file_handle, log_path)
running_processes = []

for dataset in dataset_list:
    input_dir = f"{year}{dataset}"
    output_dir = f"../../OutputFiles/{year}/{year}{dataset}"
    os.makedirs(output_dir, exist_ok=True)

    print(f"\n=== Processing dataset {dataset} ===")

    # ----------------------------
    # Step 1: Non-chamber files
    # ----------------------------
    all_root_files = glob.glob(f"{input_dir}/SingleMuon*.root")
    non_chamber_files = [
        f for f in all_root_files
        if f.endswith("_tree.root") and not any(chamber in f for chamber in chamber_list)
    ]

    if non_chamber_files:
        list_file = os.path.join(tempfile.gettempdir(), f"nonchamber_{dataset}.txt")
        with open(list_file, 'w') as f:
            f.write("\n".join(non_chamber_files))

        output_file = f"{output_dir}/csc_output_{year}{dataset}_tree.root"
        cmd = ["hadd", "-f", output_file, f"@{list_file}"]
        print(f"→ Starting non-chamber merge for dataset {dataset}...")
        # NEW: capture per-job logs
        log_path = os.path.join(tempfile.gettempdir(), f"hadd_{dataset}_nonchamber.log")
        log_f = open(log_path, "w")
        p = subprocess.Popen(cmd, stdout=log_f, stderr=log_f)
        running_processes.append((p, f"{dataset}:nonchamber", log_f, log_path))
    else:
        print("⚠️  No non-chamber files found.")

    # ----------------------------
    # Step 2: Chamber files
    # ----------------------------
    for chamber in chamber_list:
        chamber_files = glob.glob(f"{input_dir}/SingleMuon*{chamber}_tree.root")
        if chamber_files:
            list_file = os.path.join(tempfile.gettempdir(), f"{dataset}_{chamber}.txt")
            with open(list_file, 'w') as f:
                f.write("\n".join(chamber_files))

            output_file = f"{output_dir}/csc_output_{year}{dataset}_{chamber}_tree.root"
            cmd = ["hadd", "-f", output_file, f"@{list_file}"]
            print(f"→ Starting merge for chamber {chamber}...")

            # NEW: capture per-job logs
            log_path = os.path.join(tempfile.gettempdir(), f"hadd_{dataset}_{chamber}.log")
            log_f = open(log_path, "w")
            p = subprocess.Popen(cmd, stdout=log_f, stderr=log_f)
            running_processes.append((p, f"{dataset}:{chamber}", log_f, log_path))

            # Optional: limit concurrency
            while len(running_processes) >= MAX_PARALLEL_JOBS:
                # Wait for at least one to finish
                for idx, (proc, label, log_f, log_path) in enumerate(running_processes):
                    rc = proc.poll()
                    if rc is not None:
                        # NEW: on completion, close log and record status
                        log_f.close()
                        status = "OK" if rc == 0 else f"FAIL (rc={rc})"
                        summary_lines.append(f"{label}: {status}  log={log_path}")
                        running_processes.pop(idx)
                        break
                time.sleep(1)
        else:
            print(f"⚠️  No files found for chamber {chamber} in dataset {dataset}")

# =============================
# Wait for all remaining jobs
# =============================
for (proc, label, log_f, log_path) in running_processes:
    proc.wait()
    log_f.close()
    rc = proc.returncode
    status = "OK" if rc == 0 else f"FAIL (rc={rc})"
    summary_lines.append(f"{label}: {status}  log={log_path}")

# =============================
# Write summary
# =============================
with open(SUMMARY_PATH, "w") as sf:
    sf.write("hadd summary\n")
    sf.write("\n".join(summary_lines))
print(f"\n✅ All merging jobs completed. Summary -> {SUMMARY_PATH}")
