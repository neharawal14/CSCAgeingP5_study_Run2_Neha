//  void draw_mean_plot(std::vector<std::pair> mean_value_list, TString var){

//  }
#include "TGraphErrors.h"
void draw_plot(TH1D * h_clone , TString type, TString chamber_name, TString clause);
///  void draw_mean_plot(std::vector<float> mean_values_vector, std::vector<float> mean_error_values_vector, TString var, std::vector<TString> chamber_name){
///    
///    std::cout<<" started in plot mean plot "<<std::endl;
///    std::vector<float> value ={1,2,3,4,5,6,7,8,9,10,11,12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32};
///
///    int size_chamber = chamber_name.size();
///    value.resize(size_chamber);
///    //std::vector<float> value = {1,2,3, 4,5, 6, 7, 8};
///    TGraphErrors *graph_mean_values = new TGraphErrors(size_chamber, value.data(), mean_values_vector.data(), 0, mean_error_values_vector.data());
///    std::cout<<" after declaring graph "<<std::endl;
///    graph_mean_values->GetXaxis()->SetTickLength(1);
///
///    if(var=="_pressure"){
///      graph_mean_values->GetYaxis()->SetRangeUser(-0.010,0.002); 
///    }
///    else if(var=="_pressure_second") graph_mean_values->GetYaxis()->SetRangeUser(-0.0005,0.0005);
///    std::cout<<" before declaring axes "<<std::endl;
///    TAxis *axis = graph_mean_values->GetXaxis();
///    axis->Draw();
///   
///    for(int i=0; i<chamber_name.size(); i++){
///       graph_mean_values->GetXaxis()->SetBinLabel(graph_mean_values->GetXaxis()->FindBin(i + 1.), chamber_name[i]); // Find out     which bin on the x-axis the point corresponds to and set the bin label
///    }
///    graph_mean_values->GetXaxis()->SetTitleOffset(0.1); 
///    TCanvas *canv1 = new TCanvas();
///    canv1->cd();
///    canv1->SetLeftMargin(0.12);
///    canv1->SetGrid();
///    gPad->SetGrid();
///    graph_mean_values->Draw("AP");
///    graph_mean_values->SetTitle("");
///   
///    Double_t *gr_xarray = graph_mean_values->GetX();
///    Double_t *gr_yarray = graph_mean_values->GetY();
///
///    std::cout<<" before starting marker "<<std::endl;
///    std::map <TString, int> marker_colour = { {"ME11a", 8}, {"ME11b", 8}, 
///      {"ME12HV1", 2}, {"ME12HV2", 2}, {"ME12HV3", 2},
///      {"ME13HV1", 2}, {"ME13HV2", 2}, {"ME13HV3", 2},
///      {"ME21HV1", 4}, {"ME21HV2", 4}, {"ME21HV3", 4},
///      {"ME22HV1", 2},{"ME22HV2", 2},{"ME22HV3", 2},{"ME22HV4", 2},{"ME22HV5", 2},
///      {"ME31HV1", 4}, {"ME31HV2", 4}, {"ME31HV3", 4},
///      {"ME32HV1", 2},{"ME32HV2", 2},{"ME32HV3", 2},{"ME32HV4", 2},{"ME32HV5", 2},
///      {"ME41HV1", 4}, {"ME41HV2", 4}, {"ME41HV3", 4},
///      {"ME42HV1", 2},{"ME42HV2", 2},{"ME42HV3", 2},{"ME42HV4", 2},{"ME42HV5", 2}
///    };
///    //int marker_colour[32] = {8, 8, 2, 2, 2, 2,2,2, 4,4,4, 2,2,2,2,2, 4,4,4, 2,2,2,2,2, 4,4,4, 2,2,2,2,2 };
///    //int marker_colour[32] = {8, 8, 2, 2, 2, 2,2,2,  4,4,4, 2,2,2,2,2, 4,4,4, 2,2,2,2,2, 4,4,4, 2,2,2,2,2 };
///    //int marker_colour[32] = {8, 2, 2, 2, 2, 2,2, 4,4,4, 2,2,2,2,2, 4,4,4, 2,2,2,2,2, 4,4,4, 2,2,2,2,2 };
///    TLegend *legend_1 = new TLegend(0.7,0.7,0.9,0.9);
///    for (Int_t j=0; j<chamber_name.size(); j++) {
///    std::cout<<" declaring gr x arrayr "<<std::endl;
///        TMarker *m = new TMarker(gr_xarray[j], gr_yarray[j], 20);
///        m->SetMarkerColor(marker_colour[chamber_name[j]]);
///        m->Draw();
///
///      std::cout<<"after declaring gr x arrayr "<<std::endl;
///			if(j==0) {
///				legend_1->AddEntry(m," ME11a, ME11b (10^{0})","p");
///			}
///			if(j==3) {
///			legend_1->AddEntry(m," Outer Chambers (10^{0})","p"); }
///			if(j==8)
///			legend_1->AddEntry(m," Inner Chambers (20^{0})","p");
///   }
///   std::cout<<" before drawing mulitgraph "<<std::endl;
///	 legend_1->Draw("SAME");
///
///   TLatex* cmslabel_1;
///   TLatex* text1,*text2;
///   cmslabel_1 = new TLatex(0.18,0.82, "CMS #bf{#it{Preliminary}}");
///   cmslabel_1->SetNDC(kTRUE);
///   cmslabel_1->SetTextSize(0.06);
///   cmslabel_1->SetTextFont(42);
///   cmslabel_1->Draw("same");
///
///	 canv1->SaveAs("output_plots/mean_slope_values_"+var+"_run2_fit.pdf"); 
///  }

void draw_plot(TH1D * h_clone , TString type, TString chamber_name, TString clause){

  TH1D *h = (TH1D*) h_clone->Clone();
    double minBinContent = std::numeric_limits<double>::max();
    double maxBinContent = std::numeric_limits<double>::lowest();

    // Loop through all bins
    for (int bin = 1; bin <= h->GetNbinsX(); ++bin) {
        double content = h->GetBinContent(bin);
        if (content > 0) { // Ignore bins with zero content
            if (content < minBinContent) minBinContent = content;
            if (content > maxBinContent) maxBinContent = content;
        }
    }

    if (minBinContent == std::numeric_limits<double>::max()) {
        minBinContent = 1e-6; // Default small value if all bins are zero
    }

    // Find the minimum and maximum bin content
    // Optionally, adjust the range to include some margin
    double marginFactor = 0.1; // 10% margin
    double rangeMin = minBinContent - marginFactor * fabs(minBinContent);
    double rangeMax = maxBinContent + marginFactor * fabs(maxBinContent);

    // Set the y-axis range
    h->GetYaxis()->SetRangeUser(rangeMin, rangeMax);
  TCanvas *c = new TCanvas();
  c->cd();
  h->Draw();
  if(type=="time") {
  h->GetXaxis()->SetRangeUser(1462838400,1477871999);
  }
  c->SaveAs("output_plots_2016/"+clause+"/"+type+"_"+chamber_name+".pdf");
}
void reading_mean_values(){
  TString clause = "2016_second_961_pressure";
  TString input_path = "/afs/cern.ch/user/n/nrawal/eos/CSCAgeing/2024_new_plots/Comparison/time_plots/"+clause+"/";

//  std::vector<TString> chambers = {"ME32HV3", "ME11b" };
  std::vector<TString> chambers = {"ME11a", "ME11b", 
    "ME12HV1" ,"ME12HV2" , "ME12HV3",
    "ME13HV1" ,"ME13HV2" , "ME13HV3",
    "ME21HV1", "ME21HV2", "ME21HV3", 
    "ME22HV1", "ME22HV2" ,"ME22HV3","ME22HV4", "ME22HV5",
    "ME31HV1", "ME31HV2" ,"ME31HV3",
    "ME32HV1", "ME32HV2" ,"ME32HV3","ME32HV4", "ME32HV5",
    "ME41HV1", "ME41HV2" ,"ME41HV3",
    "ME42HV1", "ME42HV2" ,"ME42HV3","ME42HV4", "ME42HV5",
  };


//  std::vector<float> mean_value_list;
//  std::vector<float> mean_error_value_list;
//  std::vector<float> mean_value_second_list;
//  std::vector<float> mean_error_value_second_list;
  for(int i=0 ; i<chambers.size(); i++){ 
    TString input_file = input_path+"outf_dataset_pressure_corrected__"+chambers[i]+"_output_run2_const.root";
    std::cout<<" input file "<<input_file<<std::endl;

    TFile * file = TFile::Open(input_file, "READ");
       if (!file || file->IsZombie() || file->TestBit(TFile::kRecovered)) {
         continue;
       }
    TDirectoryFile * dir_intlumi = (TDirectoryFile*) file->Get("_integratelumi_initial");
    TDirectoryFile * dir_time = (TDirectoryFile*) file->Get("_timesecond_initial");

    if (!dir_time) {
      continue;}

    TH1D* h_pressure = (TH1D*) dir_intlumi->Get("_dataset_pressure_corrected_trimmean_allgoodchannelsvs_integratelumi_initial");
    TH1D* h_pressure_second = (TH1D*) dir_time->Get("_dataset_pressure_corrected_trimmean_allgoodchannelsvs_timesecond_initial");
    if(!h_pressure || !h_pressure_second) continue;
  draw_plot(h_pressure,"intlumi", chambers[i], clause);
  draw_plot(h_pressure_second,"time", chambers[i], clause);
    //std::cout<<" pressure entries "<<h_pressure->GetEntries()<<std::endl;

//    TF1 * f_first = (TF1*) h_pressure->GetFunction("expFit");
//    TF1 * f_second = (TF1*) h_pressure_second->GetFunction("expFit4");

 //   float mean_value = f_first->GetParameter(1);
 //   float mean_err_value = f_first->GetParError(1);

 //   float mean_value_second = f_second->GetParameter(1);
 //   float mean_err_value_second = f_second->GetParError(1);

 //   mean_value_list.push_back(mean_value);
 //   mean_error_value_list.push_back(mean_err_value);
    
 //   mean_value_second_list.push_back(mean_value_second);
 //   mean_error_value_second_list.push_back(mean_err_value_second);

    //std::cout<<" chamber "<<chambers[i]<<" parameter first "<<f_first->GetParameter(1)<<std::endl;
  }
//
//  draw_mean_plot(mean_value_list, mean_error_value_list, "_pressure", chambers);
//  draw_mean_plot(mean_value_second_list, mean_error_value_second_list, "_pressure_second", chambers);

}
