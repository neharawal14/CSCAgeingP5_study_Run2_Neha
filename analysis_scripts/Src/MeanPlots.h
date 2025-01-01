#include <iostream>
#include "TGraphErrors.h"
#include <vector>
#include "TString.h"
#include "TMarker.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TLatex.h"
using namespace std;

class MeanPlot{

public :
  void initialise(std::vector<	std::pair<float, float>>, std::vector<TString>, TString , TString, TString, TString, TString);
  void plot_mean();
  std::vector<float> mean_values_vector;
  std::vector<float> mean_error_values_vector;
  std::vector<TString> chamber_name;

  TString fit_name;
    TGraphErrors * graph_mean_values;
    TString year;
    TString var;
    TString result_path;
    TString cut_string;

};
