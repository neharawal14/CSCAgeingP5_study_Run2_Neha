import os
import subprocess
chamber_list =  ["ME11a", "ME11b", "ME12HV1", "ME12HV2","ME12HV3", "ME13HV1","ME13HV2","ME13HV3","ME21HV1","ME21HV2", "ME21HV3", "ME22HV1", "ME22HV2", "ME22HV3", "ME22HV4","ME22HV5","ME31HV1","ME31HV2", "ME31HV3", "ME32HV1", "ME32HV2", "ME32HV3", "ME32HV4","ME32HV5","ME41HV1","ME41HV2", "ME41HV3", "ME42HV1", "ME42HV2", "ME42HV3", "ME42HV4","ME42HV5"]
for chamber in chamber_list:
    subprocess.call(f"hadd 2018_all/csc_output_2018_{chamber}_tree.root 2018/2018B_1/csc_output_2018B_1_{chamber}_tree.root  2018/2018C_1/csc_output_2018C_1_{chamber}_tree.root 2018/2018A_1/csc_output_2018A_1_{chamber}_tree.root 2018/2018A_2/csc_output_2018A_2_{chamber}_tree.root 2018/2018A_3/csc_output_2018A_3_{chamber}_tree.root 2018/2018D_1/csc_output_2018D_1_{chamber}_tree.root 2018/2018D_2/csc_output_2018D_2_{chamber}_tree.root",shell=True)

#subprocess.call(f"hadd 2018_all/csc_output_2018_tree.root 2018/2018B/csc_output_2018B_tree.root 2018/2018C/csc_output_2018C_tree.root 2018/2018D/csc_output_2018D_tree.root 2018/2018E/csc_output_2018E_tree.root 2018/2018F_1/csc_output_2018F_1_tree.root 2018/2018F_2/csc_output_2018F_2_tree.root 2018/2018G/csc_output_2018G_tree.root 2018/2018H/csc_output_2018H_tree.root",shell=True)
