#include <iostream>
#include "TH1D.h"
#include "TH2D.h"
#include "TKey.h"
#include "TF1.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLatex.h"
#include <regex>
#include "TStyle.h"
using namespace std;
class individual_channels{
	public: 
	void initialise(TString, TString, TString);
	//void plot_goodchannels(TString);
  std::pair<float, float> plot_individual_channels(TString);
	//std::pair<float,float> fit_goodchannels(TString);

	TString chamber;
	TString thevar;
	TString saving_path;
	TFile *input_file;


};
