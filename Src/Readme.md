#CSC ageing studies on P5 data - Run2 with new selections

##Setup
This code is for CMSSW_13_3_0
source /cvmfs/cms.cern.ch/cmsset_default.sh cmsrel CMSSW_13_3_0 cd CMSSW_13_3_0/src cmsenv

git clone git@github.com:neharawal14/CSCAgeingP5_study_Run2_Neha.git 

The main code for ntuple selections is "AnalysisGasGain.cxx" 
Program is built using CMakeLists.txt
Make a build folder, and use make.. to build project
