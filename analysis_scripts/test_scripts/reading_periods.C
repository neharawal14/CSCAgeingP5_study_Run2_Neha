void reading_periods(){

  TString year = "2018";
  TString input_path = "/eos/home-n/nrawal/CSCAgeing/2024_new_plots/yearwise_results/new_plots_fixed/";
  TString input_path_complete = "/afs/cern.ch/work/n/nrawal/CSCAgeing_code_study/Code_and_checks/verifying_gas_gain_plots/reading_gas_gain_individual/cumulative_plots/2018_again_double_pressure/";

  std::vector<TString> chamber_list = {"ME11a" , "ME11b", "ME12HV1", "ME12HV2", "ME12HV3", 
   "ME13HV1", "ME13HV2", "ME13HV3" , 
   "ME21HV1", "ME21HV2", "ME21HV3" , 
   "ME22HV1", "ME22HV2", "ME22HV3" , "ME22HV4", "ME22HV5",
   "ME31HV1", "ME31HV2", "ME31HV3" , 
   "ME32HV1", "ME32HV2", "ME32HV3" , "ME32HV4", "ME32HV5",
   "ME41HV1", "ME41HV2", "ME41HV3" , 
   "ME42HV1", "ME42HV2", "ME42HV3" , "ME42HV4", "ME42HV5"
  }; 
 
 for(int i=0; i<chamber_list.size(); i++){
  TString chamber = chamber_list[i]; 
  TString name = "outf_dataset_pressure_corrected__"+chamber+"_output_run2_const.root"; 
  TString file_path1 = input_path+year+"_first_period_without_instlumi_cuts/"+name;
  TString file_path2 = input_path+year+"_second_period_without_instlumi_cuts/"+name;
  TString file_path3 = input_path+year+"_third_period_without_instlumi_cuts/"+name;
  //TString file_path4 = input_path+year+"_complete_period/"+name;
  TString file_path4 = input_path_complete+name;

  TFile *f1 = TFile::Open(file_path1,"READ");
  TFile *f2 = TFile::Open(file_path2,"READ");
  TFile *f3 = TFile::Open(file_path3,"READ");
  TFile *f4 = TFile::Open(file_path4,"READ");

  TString dir_name = "_pressure_"+year; 
  TDirectoryFile *pressure1 = (TDirectoryFile*) f1->Get(dir_name);
  TDirectoryFile *pressure2 = (TDirectoryFile*) f2->Get(dir_name);
  TDirectoryFile *pressure3 = (TDirectoryFile*) f3->Get(dir_name);
  //TDirectoryFile *pressure4 = (TDirectoryFile*) f4->Get(dir_name);

  TH1D* h1 = (TH1D*) pressure1->Get("_dataset_pressure_corrected_trimmean_allgoodchannelsvs_pressure");
  TH1D* h2 = (TH1D*) pressure2->Get("_dataset_pressure_corrected_trimmean_allgoodchannelsvs_pressure");
  TH1D* h3 = (TH1D*) pressure3->Get("_dataset_pressure_corrected_trimmean_allgoodchannelsvs_pressure");
  //TH1D* h4 = (TH1D*) pressure4->Get("_dataset_pressure_corrected_trimmean_allgoodchannelsvs_pressure");
  TH1D* h4 = (TH1D*) f4->Get("_dataset_pressure_corrected_trimmean_allgoodchannelsvs_pressure");

  TCanvas *c = new TCanvas();
  c->Divide(2,2);
  c->cd(1); 
  h1->Draw();
  c->cd(2); 
  h2->Draw();
  c->cd(3); 
  h3->Draw();
  c->cd(4); 
  h4->Draw();
  c->SaveAs(year+"_new/gas_gain_vs_pressure_"+chamber+".pdf");

  h4->SetMarkerStyle(20);
  h4->SetMarkerSize(0.8);
  std::vector<TH1D*> histograms = {h1, h2, h3, h4};
  std::vector<int> colors = {kMagenta, kBlue, kGreen, kRed}; // Define colors for each histogram
  std::vector<TString> periods = {"First", "Second", "Third", "Complete"};
  // Initialize the legend
TLegend *legend = new TLegend(0.1, 0.7, 0.3, 0.9); // Adjust these coordinates to fit your layout
legend->SetHeader("Periods", "C"); // Optional: Add a header to the legend
legend->SetTextSize(0.04); // Set text size
//  std::vector<float> X1 = {0.11, 0.11, 0.11, 0.11}; // Define colors for each histogram
//  std::vector<float> X2 = {0.27, 0.27, 0.27, 0.27}; // Define colors for each histogram
//  std::vector<float> Y1 = {0.70, 0.55, 0.40, 0.25}; // Define colors for each histogram
//  std::vector<float> Y2 = {0.56, 0.41, 0.26, 0.11}; // Define colors for each histogram
  std::vector<float> X1 = {0.11, 0.11, 0.75, 0.75}; // Define colors for each histogram
  std::vector<float> X2 = {0.27, 0.27, 0.89, 0.89}; // Define colors for each histogram
  std::vector<float> Y1 = {0.40, 0.25, 0.76, 0.61}; // Define colors for each histogram
  std::vector<float> Y2 = {0.26, 0.11, 0.90, 0.75}; // Define colors for each histogram

  // Now read all the histgorams and draw on same canvas, with different labels
TCanvas *c_all = new TCanvas();
 c_all->cd();
 c_all->Update();
 gStyle->SetOptStat(0);  // Disable general statistics (entries, mean, stdDev)

 gStyle->SetOptFit(1111);
 for(int i=0; i<histograms.size(); i++){
  histograms[i]->SetMarkerColor(colors[i]); 
  histograms[i]->GetYaxis()->SetRangeUser(250, 600);

      // Retrieve the fit function and make it invisible
  TF1 *fitFunc = histograms[i]->GetFunction("fa1");
    if (fitFunc) {
        fitFunc->SetLineColor(colors[i]);  // Set the line color to match the histogram
        fitFunc->SetLineStyle(0);         // Make the line invisible
        fitFunc->SetLineWidth(0);         // Alternatively, set line width to 0
    }

  //histograms[i]->GetFunction("fa1")->SetLineColor(colors[i]); 
  if(i==0)   histograms[i]->Draw("E");
  else   histograms[i]->Draw("same");
  // Enable and style statistics box
  gStyle->SetOptFit(1111);
  TPaveStats *stats = (TPaveStats*)histograms[i]->FindObject("stats");
  stats->SetX1NDC(X1[i]); // X position of top left corner
  stats->SetX2NDC(X2[i]); // X position of bottom right corner
  stats->SetY1NDC(Y1[i]); // Y position of top left corner
  stats->SetY2NDC(Y2[i]); // Y position of bottom right corner
  stats->SetTextColor(colors[i]);
//   stats->SetOptStat(1111);
   stats->Draw();
    //legend->AddEntry(histograms[i], periods[i] + " - Stat Box #" + TString::Itoa(i+1, 10), "lep");
    legend->AddEntry(histograms[i], periods[i], "lep");
  //legend->AddEntry(histograms[i], periods[i], "lep");
  c_all->Update();
 }// end of drawing histogrmas
  legend->Draw();
 c_all->SaveAs(year+"_new/gas_gain_vs_pressure_together_"+chamber+".pdf");

  } // end of for loop for chamber

}
