# To derive lumi

find DCS only file on the certificate website
https://cms-service-dqmdc.web.cern.ch/CAF/certification/

1. luminosity_2016.csv , and 2. lumibysection_2016.csv
Run over first the function "read_lumi.py" which provide a file "Integratelumi_2016.h"
Run over second function "read_lumisection.py" which provides instlumi files : put them insdie instlumi folder

# Luminosity start  for the analysis
11 May 2016
16June 2017
26 April 2018

# To find the luminosity information for all the runs which are in the DCSJson file. First setup Brilcal, and then use commands to find the integrated luminosity for each run and their lumisections
Installation brilcal : website or this code : 
Calculated luminosity information from BRIL-CAL software
brilcalc lumi -c web -r 160431
pip install brilcalc

Main Brilcal : https://twiki.cern.ch/twiki/bin/viewauth/CMS/BrilcalcQuickStart
https://cms-service-lumi.web.cern.ch/cms-service-lumi/brilwsdoc.html

Further luminosity information 
https://cmsoms.cern.ch/cms/run_3/index
Finding the luminosity goldenjson fles at : https://cms-service-dqmdc.web.cern.ch/CAF/certification/
https://twiki.cern.ch/twiki/bin/viewauth/CMS/BrilcalcQuickStart

In the analysis brilcalc is run over a single hltPathTrigger to obtain integrated luminosity corresponding to the specific trigger, not the whole golden json
We use delievered luminosity as luminosity for our analysis

code 
```
export PATH=$HOME/.local/bin:/cvmfs/cms-bril.cern.ch/brilconda/bin:$PATH
```

```
/cvmfs/cms-bril.cern.ch/brilconda310/bin/python3 -m pip install --user --upgrade brilws
```

```
normtag='/cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_PHYSICS.json'
```
Luminosity for a particular hltpath
```
 brilcalc lumi --normtag "${normtag}" --hltpath "HLT_IsoMu24_v*" -u /fb -i "Cert_294927-306462_13TeV_PromptReco_Collisions17_JSON.txt" -o 2017lumi_HLTIsoMu24.csv
 brilcalc lumi --normtag "${normtag}" --byls --hltpath "HLT_IsoMu24_v*" -u /ub -i "Cert_294927-306462_13TeV_PromptReco_Collisions17_JSON.txt" -o 2017lumi_HLTIsoMu24_byls.csv
```

Luminosity information for all the events passing all the trigger paths (not specific path)
```
 brilcalc lumi --normtag "${normtag}"  -u /fb -i "Cert_294927-306462_13TeV_PromptReco_Collisions17_JSON.txt" -o 2017lumi_DCSJson.csv
 brilcalc lumi --normtag "${normtag}" --byls -u /ub -i "Cert_294927-306462_13TeV_PromptReco_Collisions17_JSON.txt" -o 2017lumi_DCSJson_byls.csv
```

Then we use the code :  read_lumi.py and read_lumisection.py; and just make sure paths and names are correct for each year
