# Details

This code is to add the output ntuples from individual root file, to a combined root file - for a particular dataset.
Run new_add.py -> which adds all the datasets individual root file to an output root file
Change the final output folder to which your foldername  to which you want to save the output root file
And make sure you are providing correct input path to your ntuples

Adding_singlefile.py  -> This is to add all the output root files, which contain the histogram information of rechit per layer, and number of hits in a segment, ...

### Final combination

After you have root files for individual datasets, you can combine the individual datasets to final ntuples for further processing.
You can  use the command in "Combining_datasets_final/add_indie_file_2016.py"
