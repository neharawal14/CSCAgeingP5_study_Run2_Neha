import os
import subprocess
import ROOT
import argparse

parser = argparse.ArgumentParser(description='Process some inputs.')
parser.add_argument('--dataset_name', type=str, required=True, help='The name of the dataset, e.g., "2016B"')
parser.add_argument('--year', type=str, required=True, help='Year of the dataset, e.g., "2016B"')
parser.add_argument('--input_path', type=str, required=True, help='The path to the input file, e.g. "/path/to/file"')
parser.add_argument('--output_path', type=str, required=True, help='Output file path ')
args = parser.parse_args()

dataset_name = args.dataset_name
input_path = args.input_path
output_path = args.output_path
year =args.year
# specify the directory path
#dataset="B"
#input_directory = "/cmsuf/data/store/user/nrawal/rootfiles_2022/SingleMuon_2022/SingleMuon/crab_SingleMuon_Run2022{}-ZMu-PromptReco-v1".format(dataset)
#output_path= "/cmsuf/data/store/user/t2/users/neha.rawal/CSCAgeing_2022/ntuples_output_2022/2022B/"
# specify the subdirectory to ignore
#ignore_subdir = 'log'
subdir_list = []
#string_output = "list_2022_{}.txt".format(dataset)
#output_file = open(string_output,"w")
# iterate over all the directories and files

log_file=open(f"log_{year}.txt","a") 
for dirpath, dirnames, filenames in os.walk(input_path):
    for filename in filenames:
        file_name = os.path.basename(filename)
        if(file_name.endswith(".root")):
            input_file_path = os.path.join(dirpath,file_name)
            final_path = output_path+file_name
            print("final path",final_path,"\n")
            try:
                f = ROOT.TFile.Open(input_file_path)
            except IOError:
                log_file.write("Failed to open file : "+str(input_file_path)+"\n")
            else:
                print(" open file : "+str(input_file_path)+"\n")
                process_string = f"./single_file_run.sh {input_file_path} {final_path} {year}" 
                print(" process string ", process_string,"\n")
                log_file.write("process string "+process_string+"\n")
                subprocess.call(process_string,shell=True)
		#process_string = "root -l -b -q ../Src/HistMan_cxx.so ../Src/AnalysisGasGain_cxx.so \'analysisgasgain.C(0,0,\"{}\",\"{}\")\'".format(input_file_path,final_path)
