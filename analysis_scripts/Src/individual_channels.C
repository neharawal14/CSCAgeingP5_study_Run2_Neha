#include "individual_channels.h"
#include "TLegend.h"
#include "THStack.h"
#include "TLine.h"
void individual_channels :: initialise(TString chamber_name, TString input_file_name, TString saving_path_name){

	chamber = chamber_name;
	saving_path = saving_path_name;
	input_file = TFile::Open(input_file_name,"READ");
  std::cout<<" input file Name "<<input_file_name<<std::endl;
}

// SO here we analyse each individual pressure and instlumi, integratelumi gas gain 
// We make plots of Slope of gas gain across different channels, also 1D plots, also read individual channels
//
std::pair<float, float> individual_channels ::  plot_individual_channels(TString thevar){
  TString year;
  TString thevar_2 = thevar;
//  _dataset_pressure_corrected_trimmean_allgoodchannelsvs_pressure
  if(thevar=="_pressure_2016") {year = "2016"; thevar_2 ="_pressure";}
  if(thevar=="_pressure_2017") {year = "2017"; thevar_2 ="_pressure";}
  if(thevar=="_pressure_2018") {year = "2018"; thevar_2 ="_pressure";}
  if(thevar=="_pressure_2016_second") {year = "2016"; thevar_2 ="_pressure";}
  if(thevar=="_pressure_2017_second") {year = "2017"; thevar_2 ="_pressure";}
  if(thevar=="_pressure_2018_second") {year = "2018"; thevar_2 ="_pressure";}

  if(thevar=="_instlumi_2016") {year = "2016"; thevar_2 ="_instlumi";}
  if(thevar=="_instlumi_2017") {year = "2017"; thevar_2 ="_instlumi";}
  if(thevar=="_instlumi_2018") {year = "2018"; thevar_2 ="_instlumi";}
  if(thevar=="_instlumi_2016_second") {year = "2016"; thevar_2 ="_instlumi";}
  if(thevar=="_instlumi_2017_second") {year = "2017"; thevar_2 ="_instlumi";}
  if(thevar=="_instlumi_2018_second") {year = "2018"; thevar_2 ="_instlumi";}

  std::cout<<" directory to look "<<thevar<<std::endl;
	TDirectoryFile *dir = (TDirectoryFile*) input_file->Get(thevar);
  std::pair<float, float> zero = std::make_pair(0.0, 0.0);
  if (!dir) {
  std::cerr << "Error opening dir: "<< std::endl;
  return zero;
  }
 std::pair<float, float> mean_pair ;
    TString dirName = thevar;
  if (dir->GetListOfKeys()->GetSize() > 0) {
      std::cout<<" inside dir "<<dirName<<std::endl;
    // 2D histograms for slopes
    //TH1D* h1d_slope_Endcap1, *h1d_slope_Endcap2;
    TH1D* h1d_slope = nullptr;
    TH1D* h1d_chisquare = nullptr; 
    TH2D* h2d_slope_Endcap1 = nullptr, *h2d_slope_Endcap2 = nullptr;
    TH2D* h2d_chisquare_Endcap1 = nullptr, *h2d_chisquare_Endcap2 = nullptr;
    TH1D*  h_slope_inner_layers_Endcap1 = nullptr, *h_slope_outer_layers_Endcap1 = nullptr; 
    TH1D*  h_slope_inner_layers_Endcap2 = nullptr, *h_slope_outer_layers_Endcap2 = nullptr; 
 
    TH1D *h_slope_layer_Endcap1[6] = {nullptr};
    TH1D *h_slope_layer_Endcap2[6] = {nullptr};

    TH2D *h_num_entries_Endcap1 = nullptr, *h_num_entries_Endcap2 = nullptr; 

    // reading 2D entries in histogram
    std::cout<<" before accessing hist "<<dirName<<std::endl;
    h_num_entries_Endcap1 = (TH2D*) dir->Get("num_entries_2D_hist_all_bins_plus");
    h_num_entries_Endcap2 = (TH2D*) dir->Get("num_entries_2D_hist_all_bins_minus");

    std::cout<<" after accessing hist "<<dirName<<std::endl;
    // Determine the number of bins in the 2D histograms (assuming both histograms have the same binning)
    int nchamber = h_num_entries_Endcap1->GetNbinsX();
    int nlayer = h_num_entries_Endcap1->GetNbinsY();
    std::cout<<" read chamber and layers "<<dirName<<std::endl;

		int nbins;
		double entry_low_value, entry_up_value;

    std::cout<<" read all entries  first"<<std::endl;
		if(thevar_2=="_pressure" ){
     nbins = 100;
		 entry_low_value = 0 ;
		 entry_up_value = 40000 ;
		}
		else if(thevar_2=="_instlumi" ){
     nbins = 100;
		 entry_low_value = 0 ;
		 entry_up_value = 60000 ;
		}

    std::cout<<" read all entries  second"<<std::endl;
    // Create a 1D histogram with the total number of bins being nBinsX * nBinsY
    TH1D *h1d_num_entries = new TH1D("h_num_entries", "Total Entries per each cahnnel; Channel;Entries",
                                   nbins, entry_low_value,entry_up_value);

    if(chamber=="ME11a" || chamber=="ME11b" || chamber=="ME12HV1" || chamber=="ME12HV2"|| chamber=="ME12HV3" ||
       chamber=="ME13HV1" || chamber=="ME13HV2" || chamber=="ME13HV3" ||
       chamber=="ME22HV1" || chamber=="ME22HV2" || chamber=="ME22HV3" || chamber=="ME22HV4" || chamber=="ME22HV5" ||
       chamber=="ME32HV1" || chamber=="ME32HV2" || chamber=="ME32HV3" || chamber=="ME32HV4" || chamber=="ME32HV5" ||
       chamber=="ME42HV1" || chamber=="ME42HV2" || chamber=="ME42HV3" || chamber=="ME42HV4" || chamber=="ME42HV5" 
      ) {
        nchamber=36;
    }
      else if (chamber=="ME21HV1"|| chamber=="ME21HV2" || chamber=="ME21HV3" || 
               chamber=="ME31HV1"|| chamber=="ME31HV2" || chamber=="ME31HV3" || 
               chamber=="ME41HV1"|| chamber=="ME41HV2" || chamber=="ME41HV3"){

        nchamber=18;
      }
      std::cout<<" read all entries "<<std::endl;
    // Loop over all bins in the 2D histograms
    for (int i = 1; i <= nchamber; ++i) {
        for (int j = 1; j <= nlayer; ++j) {
            // Compute the bin index for the 1D histogram
            // Get the entries from both histograms for the current bin
            double entries1 = h_num_entries_Endcap1->GetBinContent(i, j);
            double entries2 = h_num_entries_Endcap2->GetBinContent(i, j);

            // Sum the entries from both histograms
            double totalEntries = entries1 + entries2;

            // Set the bin content in the 1D histogram
            h1d_num_entries->Fill(totalEntries);
        }
    }

    TCanvas *c_ent1 = new TCanvas();
    c_ent1 ->cd();
    TString title =   h_num_entries_Endcap1->GetTitle();
    TString title1 =  title+" : "+thevar+" : "+chamber;
    gStyle->SetOptStat(1111);
    h_num_entries_Endcap1->Draw("colZ text45");
    h_num_entries_Endcap1->SetTitle(title1);
    c_ent1->SaveAs(saving_path+chamber+"/num_entries_plus_endcap_"+thevar+"_"+chamber+".pdf");
    TCanvas *c_ent2 = new TCanvas();
    c_ent2 ->cd();
    gStyle->SetOptStat(1111);
    TString title2 =  h_num_entries_Endcap2->GetTitle(); 
    TString title4 = title2 +" : "+thevar+" : "+chamber;
    h_num_entries_Endcap2->Draw("colZ text45");
    h_num_entries_Endcap2->SetTitle(title4);
    c_ent2->SaveAs(saving_path+chamber+"/num_entries_minus_endcap_"+thevar+"_"+chamber+".pdf");

    TCanvas *c1_ent1 = new TCanvas();
    c1_ent1 ->cd();
    gStyle->SetOptStat(111112211);
    TString title_1 =   " Nb. of entries : "+thevar+" : "+chamber;
    h1d_num_entries->Draw();
    h1d_num_entries->SetTitle(title_1);
    h1d_num_entries->GetXaxis()->SetRangeUser(h1d_num_entries->GetBinLowEdge(0), h1d_num_entries->GetBinLowEdge(h1d_num_entries->GetNbinsX()+1));
    c1_ent1->SaveAs(saving_path+chamber+"/num_entries_1d_cumulative_"+thevar+"_"+chamber+".pdf");
 
   if(thevar_2=="_pressure"){ 
   h2d_slope_Endcap1 = new TH2D("h2d_slope_Endcap1", "Slope (pressure) vs Chamber and Layer (+endcap)", 36, 1, 37, 6, 1, 7);
   h2d_slope_Endcap2 = new TH2D("h2d_slope_Endcap2", "Slope (pressure) vs Chamber and Layer (-endcap)", 36, 1, 37, 6, 1, 7);
   //h1d_slope_Endcap1 = new TH1D("h1d_slope_Endcap1", "Slope : gas gain dependence with pressure (+endcap)", 100, -0.02, 0.01);
   //h1d_slope_Endcap2 = new TH1D("h1d_slope_Endcap2", "Slope : gas gain dependence with pressure (-endcap)", 100, -0.02, 0.01);
   h1d_slope = new TH1D("h1d_slope", "Slope : gas gain dependence with pressure ", 100, -0.02, 0.01);
   double nchamber_one = nchamber+1.0;
   h2d_chisquare_Endcap1 = new TH2D("h2d_chisquare_Endcap1", "#chi^2/ndf (pressure)  vs Chamber and Layer (+endcap)", 36, 1, nchamber_one, 6, 1, 7.0);
   h2d_chisquare_Endcap2 = new TH2D("h2d_chisquare_Endcap2", "#chi^2/ndf (pressure)  vs Chamber and Layer (-endcap)", 36, 1,  nchamber_one, 6, 1, 7.0);
   h1d_chisquare = new TH1D("h1d_chisquare", "#chi^2/ndf (pressure)", 20, 0, 20);
    std::cout<<" for one chamber "<<chamber<<std::endl;
    TString hist_name, title_name;
    for(int i=0; i<6; i++){
      hist_name = TString::Format("h_slope_layer_Endcap1[%d]", i);
      title_name =TString::Format(" pressure dependence slope for layer %d : +endcap : "+chamber,i+1) ;
      h_slope_layer_Endcap1[i] = new TH1D(hist_name,title_name  ,40,-0.012, 0); 
      title_name =TString::Format(" pressure dependence slope for layer %d : -endcap : "+chamber,i+1) ;
      hist_name = TString::Format("h_slope_layer_Endcap2[%d]", i);
      h_slope_layer_Endcap2[i] = new TH1D(hist_name, title_name ,40,-0.012, 0); 
    }
   h_slope_inner_layers_Endcap1 = new TH1D("h_slope_inner_layers_Endcap1"," pressure dependence slope for inner layers : +endcap", 40, -0.012,0);
   h_slope_outer_layers_Endcap1 = new TH1D("h_slope_outer_layers_Endcap1"," pressure dependence slope for outer layers : +endcap", 40, -0.012,0);
   h_slope_inner_layers_Endcap2 = new TH1D("h_slope_inner_layers_Endcap2"," pressure dependence slope for inner layers : -endcap", 40, -0.012,0);
   h_slope_outer_layers_Endcap2 = new TH1D("h_slope_outer_layers_Endcap2"," pressure dependence slope for outer layers : -endcap", 40, -0.012,0);
   }
   else if(thevar_2=="_instlumi"){ 
   h2d_slope_Endcap1 = new TH2D("h2d_slope_Endcap1", "Slopes (instlumi) vs Chamber and Layer (+endcap)", 36, 1, 37, 6, 1, 7);
   h2d_slope_Endcap2 = new TH2D("h2d_slope_Endcap2", "Slopes  (instlumi)vs Chamber and Layer (-endcap)", 36, 1, 37, 6, 1, 7);
   h1d_slope = new TH1D("h1d_slope", "Slopes : gas gain dependence with instlumi ", 50, -0.00003, 0.00003);
   double nchamber_one = nchamber+1;
   h2d_chisquare_Endcap1 = new TH2D("h2d_chisquare_Endcap1", "#chi^2/ndf (instlumi) vs Chamber and Layer (+endcap)", 36, 1, nchamber_one, 6, 1, 7);
   h2d_chisquare_Endcap2 = new TH2D("h2d_chisquare_Endcap2", "#chi^2/ndf (instlumi) vs Chamber and Layer (-endcap)", 36, 1, nchamber_one, 6, 1, 7);
   h1d_chisquare = new TH1D("h1d_chisquare", "#chi^2/ndf (instlumi)", 20, 0, 20);
    std::cout<<" for one chamber "<<chamber<<std::endl;
    TString hist_name, title_name;
    for(int i=0; i<6; i++){
      hist_name = TString::Format("h_slope_layer_Endcap1[%d]", i);
      title_name =TString::Format(" instlumi dependence slope for layer %d : +endcap : "+chamber,i+1) ;
      h_slope_layer_Endcap1[i] = new TH1D(hist_name,title_name  ,40,-0.00001,0.00001); 
      title_name =TString::Format(" instlumi dependence slope for layer %d : -endcap : "+chamber,i+1) ;
      hist_name = TString::Format("h_slope_layer_Endcap2[%d]", i);
      h_slope_layer_Endcap2[i] = new TH1D(hist_name, title_name ,40,-0.00001, 0.00001); 
    }
   h_slope_inner_layers_Endcap1 = new TH1D("h_slope_inner_layers_Endcap1"," instlumi dependence slope for inner layers : +endcap",50,  -0.00001, 0.00001);
   h_slope_outer_layers_Endcap1 = new TH1D("h_slope_outer_layers_Endcap1"," instlumi dependence slope for outer layers : +endcap", 50, -0.00001, 0.00001);
   h_slope_inner_layers_Endcap2 = new TH1D("h_slope_inner_layers_Endcap2"," instlumi dependence slope for inner layers : -endcap", 50, -0.00001, 0.00001);
   h_slope_outer_layers_Endcap2 = new TH1D("h_slope_outer_layers_Endcap2"," instlumi dependence slope for outer layers : -endcap", 50, -0.00001, 0.00001);
   }


    std::cout<<" after declaring histograms "<<chamber<<std::endl;
     // Get a list of all keys in the directory
    TIter next(dir->GetListOfKeys());
    TKey* key;
	
	  std::vector<float> low_ranges;
    std::vector<float> high_ranges;
    std::vector<int> channels_with_false_flag;
    std::vector<TString> channels_name_with_false_flag;
    int ch_index = 0;
		
     while ((key = (TKey*)next())) {
         // Get the object name and class name
         std::string objName = key->GetName();
         std::string className = key->GetClassName();

         // Check if the object is a histogram
         if (className.find("TH1") != std::string::npos) {
             // Read the histogram
             TH1* hist = (TH1*)key->ReadObj();
             if (hist) {
                 // Print some information about the histogram
                 std::cout << "Read histogram: " << objName
                           << " from directory: " << dirName << std::endl;
//                 TCanvas *c_hist = new TCanvas();
//                 c_hist->cd();
//                 hist->Draw();
//                 c_hist->SaveAs(saving_path+"individual"+dirName+"/"+objName+".pdf");
//                 c_hist->Close();

                 if(dirName=="_integratelumi_initial" || dirName=="_integratelumi"){
                 std::regex rgx(R"(chamber(\d+)_layer(\d+)_Endcap(\d+))");
                 std::smatch match;
                 if (std::regex_search(objName, match, rgx)) {
                     int chamber = std::stoi(match[1]);
                     int layer = std::stoi(match[2]);
                     int endcap = std::stoi(match[3]);

                    std::cout<<" hist name "<<objName<<std::endl;
                    std::cout<<" chamber "<<chamber<<" layer "<<layer<<" endcap "<<endcap<<std::endl;
                    
                    std::cout<<" beofre the flag value "<<std::endl;
                     int flag_value = hist->GetBinContent(hist->GetNbinsX()+1);
                     std::cout<<" the value of the flag for the chanber "<<flag_value<<std::endl;
                     if(flag_value==1){
                      // we will try to find the time period of int lumi for which there are entries
											 int first_bin = -1, last_bin = -1;
											for (int bin = 1; bin <= hist->GetNbinsX(); ++bin) {
											     if (hist->GetBinContent(bin) > 0) {
											         if (first_bin == -1) first_bin = bin;
											                 last_bin = bin;
											      }
											 }

												if (first_bin != -1 && last_bin != -1) {
                					float low = hist->GetXaxis()->GetBinLowEdge(first_bin);
                					float high = hist->GetXaxis()->GetBinUpEdge(last_bin);
                					low_ranges.push_back(low);
                					high_ranges.push_back(high);
                					channels_with_false_flag.push_back(ch_index);  // Store the channel index
													TString name = TString::Format("Chamber%d_layer%d_endcap%d", chamber, layer, endcap);
													channels_name_with_false_flag.push_back(name);
                          std::cout<<" found the int lumi ranges for which flag is false"<<std::endl;
													ch_index++;
                          std::cout<<" range "<<low<<" high "<<high<<" channel index "<<ch_index<<std::endl;
            						}
                     }
                     else continue;
                   }	
                 } // end of int lumi plots

                 else{
                 // Extract chamber, layer, and endcap from the histogram name using regex
                 std::regex rgx(R"(chamber(\d+)_layer(\d+)_Endcap(\d+))");
                 std::smatch match;
                 if (std::regex_search(objName, match, rgx)) {
                     int chamber = std::stoi(match[1]);
                     int layer = std::stoi(match[2]);
                     int endcap = std::stoi(match[3]);

                    std::cout<<" hist name "<<objName<<std::endl;
                    std::cout<<" chamber "<<chamber<<" layer "<<layer<<" endcap "<<endcap<<std::endl;
                     // The histogram is already fitted, just need its output values 
                     ////Fit the histogram to a straight line
                     ///TF1* fitFunc = new TF1("fitFunc", "[0] + [1]*x");
                     ///hist->Fit(fitFunc, "Q"); // Quiet fit
                     
                     if(hist->GetFunction("fa1") == NULL) continue; 
                     TF1 *fitFunc = (TF1*) hist->GetFunction("fa1");
                     std::cout<<" after obtaining fa1 "<<std::endl;
                     double slope = fitFunc->GetParameter(1);
                     double slope_err = fitFunc->GetParError(1);
                     double chi2_ndf = (fitFunc->GetChisquare()/ fitFunc->GetNDF());

                     std::cout<<" beofre the flag value "<<std::endl;
                     int flag_value = hist->GetBinContent(hist->GetNbinsX()+1);
                     std::cout<<" the value of the flag for the chanber "<<flag_value<<std::endl;
                     //if(flag_value==1){

                     // // we will try to find the time period of int lumi for which there are entries
                     // //
										 //  int first_bin = -1, last_bin = -1;
										 // for (int bin = 1; bin <= hist->GetNbinsX(); ++bin) {
										 //      if (hist->GetBinContent(bin) > 0) {
										 //          if (first_bin == -1) first_bin = bin;
										 //                  last_bin = bin;
										 //       }
										 //  }

										 // 	if (first_bin != -1 && last_bin != -1) {
                		 // 		float low = hist->GetXaxis()->GetBinLowEdge(first_bin);
                		 // 		float high = hist->GetXaxis()->GetBinUpEdge(last_bin);
                		 // 		low_ranges.push_back(low);
                		 // 		high_ranges.push_back(high);
                		 // 		channels_with_false_flag.push_back(ch_index);  // Store the channel index
										 // 		TString name = TString::Format("Chamber%d_layer%d_endcap%d", chamber, layer, endcap);
										 // 		channels_name_with_false_flag.push_back(name);
										 // 		ch_index++;
            				 // 	}
										 //                        
                     //  continue;}
										// if the flag is false
										//else{ 
                     // Store the slope in the appropriate 2D histogram
                     if (endcap == 1) {
                       // To also add further histograms for 1-6 layers and each layer separately
                        //h_slope_layer_Endcap1[layer-1]->Fill(slope, 1./slope_err);
                        h_slope_layer_Endcap1[layer-1]->Fill(slope);
                        if(layer==1 || layer==6){
                          //h_slope_inner_layers_Endcap1->Fill(slope, 1./slope_err);
                          h_slope_inner_layers_Endcap1->Fill(slope);
                        }
                        else{
                          //h_slope_outer_layers_Endcap1->Fill(slope, 1./slope_err);
                          h_slope_outer_layers_Endcap1->Fill(slope);
                        }
                         // 2D histogram for slope and chi2 for all the channels in a segment
                         // flag wheterh the chamber is an outlier
                         if(thevar_2=="_pressure"){ 
                           h2d_slope_Endcap1->SetBinContent(chamber, layer, slope*100);
                         }
                         else if(thevar_2 == "_instlumi"){
                           h2d_slope_Endcap1->SetBinContent(chamber, layer, slope*100000);
                         }
                         h1d_slope->Fill(slope);
                         h2d_chisquare_Endcap1->SetBinContent(chamber, layer, chi2_ndf);
                         h1d_chisquare->Fill(chi2_ndf);
                     } else if (endcap == 2) {

                        //h_slope_layer_Endcap2[layer-1]->Fill(slope, 1./slope_err);
                        h_slope_layer_Endcap2[layer-1]->Fill(slope);
                        if(layer==1 || layer==6){
                          //h_slope_inner_layers_Endcap2->Fill(slope, 1./slope_err);
                          h_slope_inner_layers_Endcap2->Fill(slope);
                        }
                        else{
                          //h_slope_outer_layers_Endcap2->Fill(slope, 1./slope_err);
                          h_slope_outer_layers_Endcap2->Fill(slope);
                        }

                         // 2D histogram for slope and chi2 for all the channels in a segment
                         if(thevar_2 == "_pressure"){
                         h2d_slope_Endcap2->SetBinContent(chamber, layer, slope*100);}
                         else if(thevar_2 == "_instlumi"){
                          h2d_slope_Endcap2->SetBinContent(chamber, layer, slope*100000);}
                         h1d_slope->Fill(slope);
                         h2d_chisquare_Endcap2->SetBinContent(chamber, layer, chi2_ndf);
                         h1d_chisquare->Fill(chi2_ndf);
                     }

                     std::cout<<" filled the 2D and 1D histogramsr "<<std::endl;
                     delete fitFunc;
									// }// ending the flag loop only for good channels
                 } else {
                     std::cerr << "Failed to parse histogram name: " << objName << std::endl;
                 }
             } //end of variable loop 

             } //end of hist loops
         }
     } // end of the going through directory and its histogram


    std::cout<<" access directory"<<std::endl;

    if(dirName=="_pressure_2016" || dirName=="_pressure_2017"|| dirName=="_pressure_2018" ||
        dirName=="_instlumi_2016" || dirName=="_instlumi_2017"|| dirName=="_instlumi_2018" ||
        dirName=="_pressure_2016_second" || dirName=="_pressure_2017_second"|| dirName=="_pressure_2018_second" ||
        dirName=="_instlumi_2016_second" || dirName=="_instlumi_2017_second"|| dirName=="_instlumi_2018_second"
        
        ){
    // Save the 2D histogram
      h_slope_inner_layers_Endcap1 ->SetLineColor(kRed);
      h_slope_inner_layers_Endcap1 ->SetFillColor(kRed);
      h_slope_outer_layers_Endcap1 ->SetLineColor(kBlue);
      h_slope_outer_layers_Endcap1 ->SetFillColor(kBlue);
      h_slope_inner_layers_Endcap2 ->SetLineColor(kRed);
      h_slope_inner_layers_Endcap2 ->SetFillColor(kRed);
      h_slope_outer_layers_Endcap2 ->SetLineColor(kBlue);
      h_slope_outer_layers_Endcap2 ->SetFillColor(kBlue);

      std::cout<<" inside the pressure one "<<std::endl;
      // Draw the individual histograms for lsope 
      //
      TCanvas *c_1 = new TCanvas();
      c_1->Divide(2,3);
      for(int i=0; i<6; i++){
      c_1->cd(i+1);
      h_slope_layer_Endcap1[i]->Draw();
      }
      c_1->SaveAs(saving_path+chamber+"/"+dirName+"_slope_with_layers_plus_endcap_"+thevar+"_"+chamber+".pdf");

      TCanvas *c_2 = new TCanvas();
      c_2->Divide(2,3);
      for(int i=0; i<6; i++){
      c_2->cd(i+1);
      h_slope_layer_Endcap2[i]->Draw();
      }
      c_2->SaveAs(saving_path+chamber+"/"+dirName+"_slope_with_layers_minus_endcap_"+thevar+"_"+chamber+".pdf");


    THStack hs_Endcap1("hs_Endcap1","Slope : inner vs outer layers : +endcap : "+chamber);
    hs_Endcap1.Add(h_slope_inner_layers_Endcap1);
    hs_Endcap1.Add(h_slope_outer_layers_Endcap1);

    THStack hs_Endcap2("hs_Endcap2","Slope : inner vs outer layers : -endcap : "+chamber);
    hs_Endcap2.Add(h_slope_inner_layers_Endcap2);
    hs_Endcap2.Add(h_slope_outer_layers_Endcap2);

    TCanvas *c10 = new TCanvas();
    c10->Divide(2,1);
    c10->cd(1);
    //TText T; T.SetTextFont(42); T.SetTextAlign(21);
    hs_Endcap1.Draw();
    //T.DrawTextNDC(.5,.95,"Slope : inner vs outer layers : +endcap");
    auto legend = new TLegend(0.7,0.7,0.9,0.9);
    legend->AddEntry(h_slope_inner_layers_Endcap1, " Layer 1,6");
    legend->AddEntry(h_slope_outer_layers_Endcap1, " Layer 2, 3, 4, 5");
    legend->Draw("same");
    c10->cd(2);
    //TText T1; T1.SetTextFont(42); T1.SetTextAlign(21);
    hs_Endcap2.Draw();
    //T1.DrawTextNDC(.5,.95,"Slope : inner vs outer layers : -endcap");
    //auto legend1 = new TLegend(0.1,0.7,0.48,0.9);
    auto legend1 = new TLegend(0.7,0.7,0.9,0.9);
    legend1->AddEntry(h_slope_inner_layers_Endcap2, " Layer 1,6");
    legend1->AddEntry(h_slope_outer_layers_Endcap2, " Layer 2, 3, 4, 5");
    legend1->Draw("same");
    c10->Update();
    c10->SaveAs(saving_path+chamber+"/"+dirName+"_inner_vs_outer_layers_slope_"+thevar+"_"+chamber+".pdf");

    TCanvas *c1 = new TCanvas();
    c1->cd();
    gStyle->SetOptStat(0) ;
    h2d_slope_Endcap1->Draw("colZ text45") ;
    h2d_slope_Endcap1->GetXaxis()->SetTitle("Chamber");
    h2d_slope_Endcap1->GetYaxis()->SetTitle("Layer");
    h2d_slope_Endcap1->SetTitle(" Slope : "+dirName+ " +endcap : " +chamber);
    h2d_slope_Endcap1->GetZaxis()->SetRangeUser(-2,2);
    c1->SaveAs(saving_path+chamber+"/"+dirName+"_slope_plus_endcap_"+thevar+"_"+chamber+".pdf");

    TCanvas *c2 = new TCanvas();
    c2->cd();
    gStyle->SetOptStat(0) ;
    h2d_slope_Endcap2->Draw("colZ text45") ;
    h2d_slope_Endcap2->GetXaxis()->SetTitle("Chamber");
    h2d_slope_Endcap2->GetYaxis()->SetTitle("Layer");
    h2d_slope_Endcap2->GetZaxis()->SetRangeUser(-2,2);
    h2d_slope_Endcap2->SetTitle(" Slope : "+dirName+ " -endcap : "+chamber);
    c2->SaveAs(saving_path+chamber+"/"+dirName+"_slope_minus_endcap_"+thevar+"_"+chamber+".pdf");

    // fitting slope with a gaussian 
    //
    TF1 *f1 = new TF1("f1", "gaus",-0.02,0.01);
    h1d_slope->Fit(f1);
    
    float mean_value = f1->GetParameter(1);
    float mean_error_value = f1->GetParError(1);
     mean_pair = std::make_pair(mean_value, mean_error_value);

    TCanvas *c_10 = new TCanvas();
    c_10->cd();
    gStyle->SetOptStat(111112211) ;
    gStyle->SetOptFit(1111) ;
    h1d_slope->Draw() ;
    h1d_slope->GetXaxis()->SetTitle("Slope");
    h1d_slope->SetTitle(" Slope : "+dirName+ " : " +chamber);
    c_10->SaveAs(saving_path+chamber+"/"+dirName+"_1Dslope_"+thevar+"_"+chamber+".pdf");


  /* 
    TCanvas *c_10 = new TCanvas();
    c_10->cd();
    gStyle->SetOptStat(111112211) ;
    h1d_slope_Endcap1->Draw() ;
    h1d_slope_Endcap1->GetXaxis()->SetTitle("Slope");
    h1d_slope_Endcap1->SetTitle(" Slope : "+dirName+ " +endcap : " +chamber);
    c_10->SaveAs(saving_path+dirName+"_1Dslope_plus_endcap_"+thevar+"_"+chamber+".pdf");

    TCanvas *c_20 = new TCanvas();
    c_20->cd();
    h1d_slope_Endcap2->Draw() ;
    gStyle->SetOptStat(111112211) ;
    h1d_slope_Endcap2->GetXaxis()->SetTitle("Slope");
    h1d_slope_Endcap2->SetTitle(" Slope : "+dirName+ " -endcap : "+chamber);
    c_20->SaveAs(saving_path+dirName+"_1Dslope_minus_endcap_"+thevar+"_"+chamber+".pdf");
*/

    TCanvas *c3 = new TCanvas();
    c3->cd();
    h2d_chisquare_Endcap1->Draw("colZ text45") ;
    gStyle->SetOptStat(0) ;
    h2d_chisquare_Endcap1->GetXaxis()->SetTitle("Chamber");
    h2d_chisquare_Endcap1->GetYaxis()->SetTitle("Layer");
    h2d_chisquare_Endcap1->GetZaxis()->SetRangeUser(0,20);
    h2d_chisquare_Endcap1->SetTitle(" #chi^2/ndf : "+dirName+ " +endcap : "+chamber);
    c3->SaveAs(saving_path+chamber+"/"+dirName+"_chisquare_plus_endcap_"+thevar+"_"+chamber+".pdf");

    TCanvas *c4 = new TCanvas();
    c4->cd();
    gStyle->SetOptStat(0) ;
    h2d_chisquare_Endcap2->Draw("colZ text45") ;
    h2d_chisquare_Endcap2->GetXaxis()->SetTitle("Chamber");
    h2d_chisquare_Endcap2->GetYaxis()->SetTitle("Layer");
    h2d_chisquare_Endcap2->GetZaxis()->SetRangeUser(0,20);
    h2d_chisquare_Endcap2->SetTitle(" #chi^2/ndf : "+dirName+ " -endcap : "+chamber);
    c4->SaveAs(saving_path+chamber+"/"+dirName+"_chisquare_minus_endcap_"+thevar+"_"+chamber+".pdf");

    TCanvas *c5 = new TCanvas();
    c5->cd();
    gStyle->SetOptStat(111112211) ;
    h1d_chisquare->Draw() ;
    h1d_chisquare->GetXaxis()->SetTitle("#chi^2/ndf : ");
    h1d_chisquare->SetTitle(" #chi^2/ndf : "+dirName+ " : " +chamber);
    c5->SaveAs(saving_path+chamber+"/"+dirName+"_1Dchisquare_"+thevar+"_"+chamber+".pdf");

    delete h2d_slope_Endcap1;
    delete h2d_slope_Endcap2;
    delete h2d_chisquare_Endcap1;
    delete h2d_chisquare_Endcap2;

    delete h_slope_inner_layers_Endcap1;
    delete h_slope_outer_layers_Endcap1;
    delete h_slope_inner_layers_Endcap2;
    delete h_slope_outer_layers_Endcap2;
    for(int i=0; i<6;i++){
    delete h_slope_layer_Endcap1[i];
    delete h_slope_layer_Endcap2[i];

//    gDirectory->GetList()->Remove(h_slope_layer_Endcap1[i]); 
//    gDirectory->GetList()->Remove(h_slope_layer_Endcap2[i]); 
    
    }

//    gDirectory->GetList()->Remove(h2d_slope_Endcap1); 
//    gDirectory->GetList()->Remove(h2d_slope_Endcap2); 
//    gDirectory->GetList()->Remove(h2d_chisquare_Endcap1); 
//    gDirectory->GetList()->Remove(h2d_chisquare_Endcap2); 
//    gDirectory->GetList()->Remove(h_slope_inner_layers_Endcap1); 
//    gDirectory->GetList()->Remove(h_slope_outer_layers_Endcap1); 
//    gDirectory->GetList()->Remove(h_slope_inner_layers_Endcap2); 
//    gDirectory->GetList()->Remove(h_slope_outer_layers_Endcap2); 

    }

   else if(thevar=="_integratelumi_initial" || thevar=="_integratelumi"){

      std::cout<<" came in the integratelumi Line plotting"<<std::endl;
    for (size_t i = 0; i < low_ranges.size(); ++i) {
      std::cout<<" ranges "<<low_ranges[i]<<" - "<<high_ranges[i]<<std::endl;
    }
    // Plotting the ranges
  TCanvas *c1 = new TCanvas("c1", "Integratelumi Ranges", 800, 600);
    c1->SetGrid();
    c1->SetLeftMargin(0.3);
    c1->cd();
/*
    for (size_t i = 1; i < low_ranges.size(); ++i) {
           low_ranges[i] = low_ranges[i]*(1./150);
           high_ranges[i] = high_ranges[i]*(1./150);
    } */
    // Draw a dummy histogram to set up axes
    TH1F *dummy = new TH1F("dummy", "Integratelumi Ranges for outlier channels;Integratelumi;Channel", 150, 0, 150);
    dummy->SetStats(0);
    dummy->SetMaximum(low_ranges.size() + 1); // Set max y range based on number of channels
    dummy->SetMinimum(0);                     // Set min y to 0
    dummy->Draw();  // Draw the dummy histogram to set up the axes

//        TLine *line = new TLine(low_ranges[0], 1, high_ranges[0],  1);
//        line->SetLineColor(kRed);
//        line->Draw();

    for (size_t i = 0; i < low_ranges.size(); ++i) {
        TLine *line = new TLine(low_ranges[i], i + 1, high_ranges[i], i + 1);
        line->SetLineColor(kRed);
        line->Draw("same");
    }
    // Set Y-axis labels to channel names
    for (size_t i = 0; i < channels_with_false_flag.size(); ++i) {
        int channel_index = channels_with_false_flag[i];
        if (channel_index < channels_name_with_false_flag.size()) {
            TLatex *label = new TLatex(-5, i + 1, channels_name_with_false_flag[channel_index]);
            label->SetTextAlign(32);
            label->SetTextSize(0.02);
            label->Draw();
        }
    } 



    c1->SaveAs(saving_path+chamber+"/"+thevar+"_ranges_"+chamber+".pdf");
}

}// end of directory and histogram reading

// Return the function 
return mean_pair;
}
// end of individual channel plotting 
