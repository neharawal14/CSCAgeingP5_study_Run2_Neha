#include "HistMan.h"
#include "AnalysisGasGain.h"
#include "TMath.h"
#include "TBranch.h"  
#include "TTree.h"
#include "TFile.h"
#include "string"
#include "sstream"
#include <vector>
#include <iostream>
#include "TClassTable.h"
#include "TROOT.h"
#include "TString.h"
using namespace std;

int main(int argc, char *argv[]) {
//  gSystem->Load("../Src/HistMan_cxx.so");
//  gSystem->Load("../Src/AnalysisGasGain_cxx.so");
 
  int fstat = 0; 
  int fprint = 0; 
  string ntuplename = argv[1]; 
  string histroot = argv[2]; 
  string year = argv[3];
  
  HistMan *histos = new HistMan();
  AnalysisGasGain *anl=new AnalysisGasGain();
 //ntuplename.replace(ntuplename.begin(),ntuplename.begin()+9,"");
  //string prefix = "root://cmsio2.rc.ufl.edu/";
//  ntuplename = prefix+ntuplename;
  std::cout << ntuplename <<std::endl;
 
  anl->Setup(fstat,fprint,ntuplename,histroot, year);
  anl->SetupTree();
  anl->Analyze(histos);

  delete anl;

return 0;
 }
