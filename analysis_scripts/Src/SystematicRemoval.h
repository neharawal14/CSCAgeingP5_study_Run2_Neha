#ifndef SYS_REMOVAL_H
#define SYS_REMOVAL_H

#include <iostream>
#include "TH1D.h"
#include "TString.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TStyle.h"
#include "TPaveText.h"
class SystematicRemoval {

  public:

    TFile *input_file_org, *input_file_ref;
    TString chamber_name, normalising_chamber, saving_path;
    TString input_path;
    void initialise(TString, TString, TString);
    void ratio(TString);
    std::pair<float, float> normalised_fit(TString);
    void adding_plus_minus(TString chamber_name);
    //TH1D *h_normalised_plus, *h_normalised_minus;
    //TH1D *h_normalised_plus_first, *h_normalised_minus_first;
    TH1D* h_normalised_avg;
    TH1D* h_normalised_avg_first;
    TH1D* h_goodchannels_avg;
    TH1D* h_goodchannels_avg_ref;
 
    bool flag;
};
#endif
