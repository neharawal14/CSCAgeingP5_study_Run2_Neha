# To process ntuples to make final root file, with histograms of -
Gas gain dependence on pressure, instlumi, intlumi

### To produce the output root files use the scripts in Src/running_scripts/

# To make final plot of gas gain dependence on pressure and instlumi, intlumi :
Use the program analysis_script/Src/

CF4 changes to 10% in different years -
2016 - March 30; http://cmsonline.cern.ch/cms-elog/9112301
2017 - April 24; http://cmsonline.cern.ch/cms-elog/977251a
2018 - March 29; http://cmsonline.cern.ch/cms-elog/1033819

# New commit I updated following things

1. Updated inclusion of gas gain on time
2. Removed the constraint of having 100 entries per bin per channel
3. Trimming 15% of the tail including even the overflow entries above (charge > 3000) also in the tail (before trimming)
4. Removing recuperated periods from 2018 (during which gas gain drops from normal)
5. Removed the period during 2017 where gas composition changed (7 Aug- 19Aug)
