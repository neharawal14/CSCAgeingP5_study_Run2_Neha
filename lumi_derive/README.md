# To derive lumi

find DCS only file on the certificate website
https://cms-service-dqmdc.web.cern.ch/CAF/certification/

// User Brilcal to calculate the lumi for your data
Setup Brilcal

export PATH=$HOME/.local/bin:/cvmfs/cms-bril.cern.ch/brilconda/bin:$PATH

/cvmfs/cms-bril.cern.ch/brilconda3/bin/python3 -m pip install --user --upgrade brilws

normtag='/cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_BRIL.json'

brilcalc lumi --normtag "${normtag}" -u /fb -i "$(curl -s "${golden_jsonB}")"

To RUn ::

Provide two input files : 
1. luminosity_2016.csv , and 2. lumibysection_2016.csv
Run over first the function "read_lumi.py" which provide a file "Integratelumi_2016.h"
Run over second function "read_lumisection.py" which provides instlumi files : put them insdie instlumi folder
