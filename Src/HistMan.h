#ifndef ROOT_HistMan
#define ROOT_HistMan

#include "TFile.h"
#include <string>
#include "TH1D.h"
#include "TH2D.h"
#include "TH1F.h"
#include "TH2F.h"
using namespace std;
//#include <algorithm>

class HistMan : public TObject {

public:

  HistMan();
  virtual ~HistMan();

  // write hists to file with folders
  void writeHists(TFile *);

  // fill 1-Dim histograms 

  void fill1DHist(float,std::string,std::string,std::string,std::string,int,int,float,float,float,std::string);
  void fill1FHist(float,std::string,std::string,std::string,std::string,int,int,float,float,float,std::string);

  // fill 2-Dim histograms

  void  fill2DHist(float,float,std::string,std::string,std::string,std::string,std::string,int,float,float,int,float,float,float,std::string);
  void  fill2FHist(float,float,std::string,std::string,std::string,std::string,std::string,int,float,float,int,float,float,float,std::string);

  // get 1-Dim histograms
  TH1D* get1DHist(std::string);
  TH1F* get1FHist(std::string);

  // get 2-Dim histograms
  TH2D* get2DHist(std::string);
  TH2F* get2FHist(std::string);

  // Clear maps with histograms (called after saving histogram to file)
  void ClearHistMaps();

ClassDef(HistMan,1) 
  };

#endif
