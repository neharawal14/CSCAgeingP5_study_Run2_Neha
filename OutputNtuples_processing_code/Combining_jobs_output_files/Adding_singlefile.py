import ROOT
import glob

# Grab all ROOT files in the directory
#dataset_list = ["2016D_1", "2016D_2", "2016D_3", "2016E_1", "2016E_2",  "2016F_1", "2016F_2", 
#         "2016F_3", "2016G_1", "2016G_2", "2016G_3", "2016H_1", "2016H_2"
#        ]
dataset_list = ["Single_files"] 
for directory_name in dataset_list:
    all_files = glob.glob(f"{directory_name}/*.root")
    
    # Filter out files that contain 'ME11' or 'tree' in their name
    input_files = [f for f in all_files if "ME11" not in f and "tree" not in f]
    
    print("Files selected for histogram summing:")
    #for f in input_files:
    #    print(" -", f)
    
    # Dictionary to accumulate summed histograms
    final_hists = {}
    
    # Loop through each file
    for filename in input_files:
        f = ROOT.TFile.Open(filename)
        if not f or f.IsZombie():
            print(f" Failed to open: {filename}")
            continue
    
        test_dir = f.Get("Test")
        if not test_dir:
            print(f"No 'Test' directory in: {filename}")
            f.Close()
            continue
    
        # Loop through keys in 'Test' directory
        for key in test_dir.GetListOfKeys():
            obj = key.ReadObj()
            if not obj.InheritsFrom("TH1"):
                continue  # skip non-histograms
    
            hist_name = obj.GetName()
    
            if hist_name not in final_hists:
                final_hists[hist_name] = obj.Clone()
                final_hists[hist_name].SetDirectory(0)
            else:
                final_hists[hist_name].Add(obj)
    
        f.Close()
    
    # Write all summed histograms to output file
    output_file = ROOT.TFile(f"summed_histograms_{directory_name}.root", "RECREATE")
    output_dir = output_file.mkdir("Test")
    output_dir.cd()
    
    for hist in final_hists.values():
        hist.Write()
    
    output_file.Close()
    print("Done! Output saved to: summed_histograms_filtered.root")
