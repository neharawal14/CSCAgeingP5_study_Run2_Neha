#include "HistMan.h"
#include "TObject.h"
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>
#include "TFile.h"
#include "TH1.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TH1F.h"
#include "TH2F.h"
#include <algorithm>

#include "TTree.h"

ClassImp(HistMan)

 HistMan::HistMan() { }
 HistMan::~HistMan() { }

  // maps to hold histograms
  std::map<std::string,std::pair<TH1D*,std::string> > theMapTH1D;
  std::map<std::string,std::pair<TH2D*,std::string> > theMapTH2D;
  std::map<std::string,std::pair<TH1F*,std::string> > theMapTH1F;
  std::map<std::string,std::pair<TH2F*,std::string> > theMapTH2F;

 void HistMan::writeHists(TFile* theFile, Long64_t nTotal, Long64_t nPassed, Long64_t used_events,  Long64_t totalHits){
		theFile->cd();
		
		// Make a 1-entry summary tree
		TTree summary("SummaryTree","Job / file summary");
		
		Long64_t NTotal   = nTotal;
		Long64_t NPassed  = nPassed;
		Long64_t TotalHits = totalHits;
		Long64_t UsedEvents = used_events;
		
		summary.Branch("nTotal",    &NTotal,    "nTotal/L");
		summary.Branch("nPassed",   &NPassed,   "nPassed/L");
		summary.Branch("totalHits", &TotalHits, "totalHits/L");
		summary.Branch("used_events", &UsedEvents, "used_events/L");
		
		summary.Fill();
		summary.Write();   // writes into the currently open file
	}
  // write hists to file with folders
  void HistMan::writeHists(TFile* theFile){
    std::vector<std::string> theFolders;
    std::vector<std::string>::iterator fit;
    theFile->cd();

    std::map<std::string,std::pair<TH1D*,string> >::const_iterator mapith1d;
    for (mapith1d = theMapTH1D.begin(); mapith1d != theMapTH1D.end(); mapith1d++){
      std::string folder = (*mapith1d).second.second.c_str();
      fit = find(theFolders.begin(), theFolders.end(), folder);
      if (fit == theFolders.end()){
       theFolders.push_back(folder);
       theFile->mkdir(folder.c_str());
      }
      theFile->cd((*mapith1d).second.second.c_str());
      (*mapith1d).second.first->Write();
      theFile->cd();
    }

    std::map<std::string,std::pair<TH1F*,string> >::const_iterator mapith1f;
    for (mapith1f = theMapTH1F.begin(); mapith1f != theMapTH1F.end(); mapith1f++){
      std::string folder = (*mapith1f).second.second.c_str();
      fit = find(theFolders.begin(), theFolders.end(), folder);
      if (fit == theFolders.end()){
        theFolders.push_back(folder);
        theFile->mkdir(folder.c_str());
      }
      theFile->cd((*mapith1f).second.second.c_str());
      (*mapith1f).second.first->Write();
      theFile->cd();
    }


    std::map<std::string,std::pair<TH2D*,string> >::const_iterator mapith2d;
    for (mapith2d = theMapTH2D.begin(); mapith2d != theMapTH2D.end(); mapith2d++){
      std::string folder = (*mapith2d).second.second.c_str();
      fit = find(theFolders.begin(), theFolders.end(), folder);
      if (fit == theFolders.end()){
        theFolders.push_back(folder);
        theFile->mkdir(folder.c_str());
      }
      theFile->cd((*mapith2d).second.second.c_str());
      (*mapith2d).second.first->Write();
      theFile->cd();
    }

    std::map<std::string,std::pair<TH2F*,string> >::const_iterator mapith2f;
    for (mapith2f = theMapTH2F.begin(); mapith2f != theMapTH2F.end(); mapith2f++){
      std::string folder = (*mapith2f).second.second.c_str();
      fit = find(theFolders.begin(), theFolders.end(), folder);
      if (fit == theFolders.end()){
        theFolders.push_back(folder);
        theFile->mkdir(folder.c_str());
      }
      theFile->cd((*mapith2f).second.second.c_str());
      (*mapith2f).second.first->Write();
      theFile->cd();
    }
  }

  // fill 1-Dim histogram 
  void HistMan::fill1DHist(float x, std::string name, std::string title,std::string xtitle,std::string ytitle,int fillcol,int bins, float xmin, float xmax, float weight,std::string folder) {
    if (theMapTH1D.find(name) == theMapTH1D.end()) {
      theMapTH1D[name] = std::pair<TH1D*,string>(new TH1D(name.c_str(),title.c_str(),bins,xmin,xmax),folder);
      theMapTH1D[name].first->GetXaxis()->SetTitle(xtitle.c_str());
      theMapTH1D[name].first->GetYaxis()->SetTitle(ytitle.c_str());
      theMapTH1D[name].first->SetFillColor(fillcol);
    }
    theMapTH1D[name].first->Fill(x,weight);
  }

  void HistMan::fill1FHist(float x, std::string name, std::string title,std::string xtitle,std::string ytitle,int fillcol,int bins, float xmin, float xmax, float weight,std::string folder) {
    if (theMapTH1F.find(name) == theMapTH1F.end()) {
      theMapTH1F[name] = std::pair<TH1F*,string>(new TH1F(name.c_str(),title.c_str(),bins,xmin,xmax),folder);
      theMapTH1F[name].first->GetXaxis()->SetTitle(xtitle.c_str());
      theMapTH1F[name].first->GetYaxis()->SetTitle(ytitle.c_str());
      theMapTH1F[name].first->SetFillColor(fillcol);
    }
    theMapTH1F[name].first->Fill(x,weight);
  }

  // fill 2D histogram

  void  HistMan::fill2DHist(float x, float y, std::string name, std::string title,std::string xtitle,std::string ytitle,std::string option,int binsx, float xmin, float xmax,int binsy, float ymin, float ymax, float weight,std::string folder) {
    if (theMapTH2D.find(name) == theMapTH2D.end()) {
      theMapTH2D[name] = std::pair<TH2D*,string>(new TH2D(name.c_str(),title.c_str(),binsx,xmin,xmax,binsy,ymin,ymax),folder);
      theMapTH2D[name].first->GetXaxis()->SetTitle(xtitle.c_str());
      theMapTH2D[name].first->GetYaxis()->SetTitle(ytitle.c_str());
      theMapTH2D[name].first->SetOption(option.c_str());
    }
   theMapTH2D[name].first->Fill(x,y,weight);
}

  void  HistMan::fill2FHist(float x, float y, std::string name, std::string title,std::string xtitle,std::string ytitle,std::string option,int binsx, float xmin, float xmax,int binsy, float ymin, float ymax, float weight,std::string folder) {
    if (theMapTH2F.find(name) == theMapTH2F.end()) {
      theMapTH2F[name] = std::pair<TH2F*,string>(new TH2F(name.c_str(),title.c_str(),binsx,xmin,xmax,binsy,ymin,ymax),folder);
      theMapTH2F[name].first->GetXaxis()->SetTitle(xtitle.c_str());
      theMapTH2F[name].first->GetYaxis()->SetTitle(ytitle.c_str());
      theMapTH2F[name].first->SetOption(option.c_str());
    }
   theMapTH2F[name].first->Fill(x,y,weight);
}


  // get 1-Dim histogram
  TH1D* HistMan::get1DHist(std::string name) {
     if (theMapTH1D.find(name) != theMapTH1D.end()) return theMapTH1D[name].first;
     else return NULL;
  }

  TH1F* HistMan::get1FHist(std::string name) {
     if (theMapTH1F.find(name) != theMapTH1F.end()) return theMapTH1F[name].first;
     else return NULL;
  }

  // get 2-Dim histogram
  TH2D* HistMan::get2DHist(std::string name) {
     if (theMapTH2D.find(name) != theMapTH2D.end()) return theMapTH2D[name].first;
     else return NULL;
  }

  TH2F* HistMan::get2FHist(std::string name) {
     if (theMapTH2F.find(name) != theMapTH2F.end()) return theMapTH2F[name].first;
     else return NULL;
  }

  // Remove histograms
void HistMan::ClearHistMaps() {

 theMapTH1D.clear();
 theMapTH2D.clear();
 theMapTH1F.clear();
 theMapTH2F.clear();
}
