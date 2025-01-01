#include "CumulativePlots.h"
#include "TLegend.h"
#include "THStack.h"
#include "TLine.h"
void CumulativePlot :: initialise(TString chamber_name, TString input_file_name, TString saving_path_name){

	chamber = chamber_name;
	saving_path = saving_path_name;
	input_file = TFile::Open(input_file_name,"READ");
  std::cout<<" input file Name "<<input_file_name<<std::endl;
}
TString CumulativePlot :: get_string(){

  TString cut_string;

    // Get the list of keys in the root directory
    TDirectoryFile *dir = (TDirectoryFile*) input_file->Get("_integratelumi");
    TList* keyList = dir->GetListOfKeys();
    // Iterate over all keys
    TIter nextKey(keyList);
    TKey* key;
    while ((key = (TKey*)nextKey())) {
        // Get the class name of the object
        TString className = key->GetClassName();
        
        // Check if the object is a TNamed
        if (className == "TNamed") {
            // Retrieve the object
            TNamed* namedObject = (TNamed*)key->ReadObj();
            if (namedObject) {
              cut_string= namedObject->GetTitle();
                // Print the title string of the TNamed object
                std::cout << "TNamed Object Found:" << std::endl;
                std::cout << "  Name: " << namedObject->GetName() << std::endl;
                std::cout << "  Title: " << namedObject->GetTitle() << std::endl;
            }
        }
    }
  
  return cut_string;
}

// SO here we analyse each individual pressure and instlumi, integratelumi gas gain 
// We make plots of Slope of gas gain across different channels, also 1D plots, also read individual channels
//

// end of individual channel plotting 
//
// plotting all the good channels
void CumulativePlot ::  plot_goodchannels(TString thevar){

  TString year;
  TString thevar_2 = thevar;
  TLatex *cmslabel_1;
  TLatex *cmslabel_2;
  TLatex *text1;
  TLatex *text2;
  TLatex *text1_2;
  TLatex *text2_2;

  if(thevar=="_pressure_2016") {year = "2016"; thevar_2 ="_pressure";}
  else if(thevar=="_pressure_2017") {year = "2017"; thevar_2 ="_pressure";}
  else if(thevar=="_pressure_2018") {year = "2018"; thevar_2 ="_pressure";}
  else if(thevar=="_instlumi_2016") {year = "2016"; thevar_2 ="_instlumi";}
  else if(thevar=="_instlumi_2017") {year = "2017"; thevar_2 ="_instlumi";}
  else if(thevar=="_instlumi_2018") {year = "2018"; thevar_2 ="_instlumi";}
  else if(thevar=="_pressure_2016_second") {year = "2016"; thevar_2 ="_pressure";}
  else if(thevar=="_pressure_2017_second") {year = "2017"; thevar_2 ="_pressure";}
  else if(thevar=="_pressure_2018_second") {year = "2018"; thevar_2 ="_pressure";}
  else if(thevar=="_instlumi_2016_second") {year = "2016"; thevar_2 ="_instlumi";}
  else if(thevar=="_instlumi_2017_second") {year = "2017"; thevar_2 ="_instlumi";}
  else if(thevar=="_instlumi_2018_second") {year = "2018"; thevar_2 ="_instlumi";}


		TDirectoryFile *dir = (TDirectoryFile*) input_file->Get(thevar);
    if(!dir){
      return;
    }
    if (dir->GetListOfKeys()->GetSize() > 0) {
    std::cout<<" access directory"<<std::endl;
		TH1D * h_goodchannels_plus, *h_goodchannels_minus;
    TString  histogram_plus_name = "_dataset_pressure_corrected_trimmean_allgoodchannelsvs"+thevar_2;
		h_goodchannels_plus = (TH1D*) dir->Get(histogram_plus_name);

    std::cout<<" after hist "<<std::endl;

    std::cout<<" entries in histogram "<<h_goodchannels_plus->GetEntries()<<std::endl;
	if(thevar_2=="_pressure" || thevar_2=="instlumi"){
   h_goodchannels_plus->GetYaxis()->SetRangeUser(300,600);
	}	
	if(thevar_2=="_integratelumi" || thevar_2 =="_integratelumi_initial"){
   h_goodchannels_plus->GetYaxis()->SetRangeUser(250,600);
	}	

	TString saving_name_plus, saving_name_minus;	

  std::cout<<" setting range for hist "<<std::endl;
  std::cout<<" after canvas declaration "<<std::endl;
  
  TString title_plus, title_minus;
  if(thevar_2=="_pressure" || thevar_2=="_instlumi"){
  title_plus = "cumulative : "+chamber+year+ " : "+thevar_2;
	saving_name_plus = saving_path+"allgoodchannels/"+chamber+"/cumulative_"+chamber+thevar+"_avg_"+year+".pdf";
  }
  else{
  title_plus = "cumulative : "+chamber+thevar_2;
	saving_name_plus = saving_path+"allgoodchannels/"+chamber+"/cumulative_"+chamber+thevar+"_avg.pdf";
  }

  std::cout<<" before drawing histogram "<<saving_name_plus<<std::endl;
		TCanvas * c = new TCanvas();
		c->cd();
    c->SetLeftMargin(0.12);
    c->SetBottomMargin(0.12);
    h_goodchannels_plus->GetYaxis()->CenterTitle(true);
		h_goodchannels_plus->Draw();
    h_goodchannels_plus->SetTitle(title_plus);
   cmslabel_1 = new TLatex(0.22,0.81, "CMS #bf{#it{Preliminary}}");
   cmslabel_1->SetNDC(kTRUE);
   cmslabel_1->SetTextSize(0.06);
   cmslabel_1->SetTextFont(42);
   cmslabel_1->Draw("same");
    if(thevar_2=="_integratelumi" || thevar_2=="_integratelumi_initial"){ 
     text1 = new TLatex(0.68,0.91, "pp, 13 TeV");
     text1->SetNDC(kTRUE);
     text1->SetTextSize(0.06);
     text1->SetTextFont(42);
     text1->Draw("same"); 
    }
    else if(thevar_2=="_pressure" || thevar_2=="_instlumi"){
     if(thevar=="_pressure_2016" || thevar=="_instlumi_2016" || thevar=="_pressure_2016_second" || thevar=="_instlumi_2016_second") { 
      //text2 = new TLatex(0.46,0.72 , "7000 (#mu b.s)^{-1} #leq L_{inst} #leq 10000 (#mu b.s)^{-1}");
      text1 = new TLatex(0.56,0.91, "pp, L= 40fb^{-1}(13 TeV)");

    }
     if(thevar=="_pressure_2017" || thevar=="_instlumi_2017" || thevar=="_pressure_2017_second" || thevar=="_instlumi_2017_second") { 
      text1 = new TLatex(0.56,0.91, "pp, L= 44fb^{-1}(13 TeV)");
     // text2 = new TLatex(0.46,0.72 , "7000 (#mu b.s)^{-1} #leq L_{inst} #leq 10000 (#mu b.s)^{-1}");
    }
     if(thevar=="_pressure_2018" || thevar=="_instlumi_2018" || thevar=="_pressure_2018_second" || thevar=="_instlumi_2018_second") { 
      text1 = new TLatex(0.56,0.91, "pp, L= 65fb^{-1}(13 TeV)");
      //text2 = new TLatex(0.46,0.72 , "10000 (#mu b.s)^{-1} #leq L_{inst} #leq 15000 (#mu b.s)^{-1}");
    }
//     text2->SetTextFont(42); text2->SetTextSize(0.04);
//      text2->SetNDC(kTRUE);
//      text2->Draw("same");
    
      text1->SetTextFont(42);
      text1->SetTextSize(0.06);
      text1->SetNDC(kTRUE);
      text1->Draw("same");
    }

		c->SaveAs(saving_name_plus);
		c->Close();

    }// ending of directory and histogram plotting
    std::cout<<" after canvas declaration "<<std::endl;
}

std::pair<float, float> CumulativePlot ::  fit_goodchannels(TString thevar){
	TString thevar_2 = thevar;
  if(thevar=="_pressure_2016") {thevar_2 ="_pressure";}
  if(thevar=="_pressure_2017") {thevar_2 ="_pressure";}
  if(thevar=="_pressure_2018") {thevar_2 ="_pressure";}
  if(thevar=="_pressure_2016_second") {thevar_2 ="_pressure";}
  if(thevar=="_pressure_2017_second") {thevar_2 ="_pressure";}
  if(thevar=="_pressure_2018_second") {thevar_2 ="_pressure";}

	if(thevar=="_instlumi_2016") {thevar_2 ="_instlumi";}
  if(thevar=="_instlumi_2017") {thevar_2 ="_instlumi";}
  if(thevar=="_instlumi_2018") {thevar_2 ="_instlumi";}
	if(thevar=="_instlumi_2016_second") {thevar_2 ="_instlumi";}
  if(thevar=="_instlumi_2017_second") {thevar_2 ="_instlumi";}
  if(thevar=="_instlumi_2018_second") {thevar_2 ="_instlumi";}

    std::pair<float, float> mean_slope;
  	TDirectoryFile *dir = (TDirectoryFile*) input_file->Get(thevar);
    std::cout<<" reading dir "<<std::endl;
    if(!dir){
      mean_slope.first = 0; 
      mean_slope.second = 0;  
      return mean_slope;
    }
     if (dir->GetListOfKeys()->GetSize() <= 0) {
      mean_slope.first = 0; 
      mean_slope.second = 0;  
      return mean_slope;
     }
     std::cout<<" coming further in dir"<<std::endl;  
    TH1D * h_goodchannels_plus, *h_goodchannels_minus;
    TString  histogram_plus_name = "_dataset_pressure_corrected_trimmean_allgoodchannelsvs"+thevar_2;
		h_goodchannels_plus = (TH1D*) dir->Get(histogram_plus_name);
	
	 TF1 *fitResult_plus = h_goodchannels_plus->GetFunction("fa1");
	 std::cout<<" print parameter "<<fitResult_plus->GetParameter(1);	
	 std::cout<<" print parameter "<<fitResult_plus->GetParError(1);	
  std::pair<float, float> mean_plus;
	float slope_plus, slope_error_plus;
	slope_plus = fitResult_plus->GetParameter(1);	
	slope_error_plus =  fitResult_plus->GetParError(1);	

	mean_plus.first= slope_plus;
	mean_plus.second= slope_error_plus;

  mean_slope.first = slope_plus;
  mean_slope.second = slope_error_plus;
  std::cout<<" mean values "<<mean_slope.first<<" errror "<<mean_slope.second<<std::endl;
	return mean_slope;
/*
		TCanvas * c = new TCanvas();
		c->cd();
		h_goodchannels_plus->Draw();
		saving_name = saving_path+"cumulative_"+chamber+thevar+"_plus.pdf";
		c->SaveAs(saving_name);
		c->Close();

		TCanvas * c1 = new TCanvas();
		c1->cd();
		h_goodchannels_minus->Draw();
		saving_name = saving_path+"cumulative_"+chamber+thevar+"_minus.pdf";
		c1->SaveAs(saving_name);
		c1->Close();
*/
}
