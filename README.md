# CSC ageing studies on P5 data - Run2 with new selections

## Setup
This code is for CMSSW_15_0_4
```source /cvmfs/cms.cern.ch/cmsset_default.sh cmsrel CMSSW_15_0_4 
cd CMSSW_15_0_4/src 
cmsenv
```

## Checkout the branch  Dev_Run2_code
```
git remote add origin  git@github.com:neharawal14/CSCAgeingP5_study_Run2_Neha.git 
git fetch origin Run2_NewSelection_updates
git checkout Run2_NewSelection_updates
```
or checkout in this manner
```
git clone --branch Run2_NewSelection_updates --single-branch git@github.com:neharawal14/CSCAgeingP5_study_Run2_Neha.git 
```

## Code structure
The main code for ntuple selections is "AnalysisGasGain.cxx" 
Files needed :  Pressure information (pressurecsc_20.h) , Luminosity information (Integratelumi_20.h), HV equalisation (ChargeORIGandInstL.C) , main 
Executable can be built using CMakeLists.txt
This CMakeLists.txt also uses LinkDef.h 

Further, the instlumi information should be stored in the folder : files_HVandLumi/InstLumiPerRun/20/

To build project , make a build directory
```mkidr build
cd build
```
Make your executable
```cmake ..
	 make
```
## Run the executable with the script in "WORK" folder : "Work/single_file_run.sh"

In my current hipergator I need to change the prefix of my CMAKE_PREFIX_PATH to the root I use.
Find MY_ROOT_PATH using
``` 
which root
```
Update prefix path  : 
```
export CMAKE_PREFIX_PATH=MY_ROOT_PATH:$CMAKE_PREFIX_PATH
```

