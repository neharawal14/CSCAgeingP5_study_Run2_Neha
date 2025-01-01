#include "MeanPlots.h"

void MeanPlot :: initialise(std::vector<	std::pair<float, float>> mean_vector,
   std::vector<TString> chamber, TString year_value , TString var_name, TString fit_type, TString save_path, TString cut_string_name){
    fit_name = fit_type;
    var= var_name;
    cut_string= cut_string_name;
    //result_path="../results/results_"+year_value+"_wider_new/MeanPlots/";
    result_path=save_path;
    //result_path="../results/results_"+year_value+"_LThomas_range/MeanPlots/";
    year = year_value;
      for(int i=0; i< chamber.size(); i++) {
         mean_values_vector.push_back(mean_vector[i].first); 
         mean_error_values_vector.push_back(mean_vector[i].second); 
         chamber_name.push_back(chamber[i]); 
      }

      for(int i=0; i< chamber.size(); i++) {
        std::cout<<" inside function mean "<<chamber_name[i]<<" : "<<mean_values_vector[i]<<std::endl;
        std::cout<<" inside function mean error "<<chamber_name[i]<<" : "<<mean_error_values_vector[i]<<std::endl;
      } 
    }

// end of initialisation function 

  void MeanPlot :: plot_mean(){
    
    std::cout<<" started in plot mean plot "<<std::endl;
    std::vector<float> value ={1,2,3,4,5,6,7,8,9,10,11,12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32};

    int size_chamber = chamber_name.size();
    value.resize(size_chamber);
    //std::vector<float> value = {1,2,3, 4,5, 6, 7, 8};
    graph_mean_values = new TGraphErrors(chamber_name.size(), value.data(), mean_values_vector.data(), 0, mean_error_values_vector.data());
    std::cout<<" after declaring graph "<<std::endl;
    graph_mean_values->GetXaxis()->SetTickLength(1);

    if(var=="_pressure"){
      if(year=="2017"){
      graph_mean_values->GetYaxis()->SetRangeUser(-0.010,0.002); 
      }
      else{
      graph_mean_values->GetYaxis()->SetRangeUser(-0.010,0.002); 
      }
    }
    else if(var=="_pressure_second") graph_mean_values->GetYaxis()->SetRangeUser(-0.0005,0.0005);
    else if(var=="_instlumi")    graph_mean_values->GetYaxis()->SetRangeUser(-0.00001,0.00001); 
    else if(var=="_instlumi_second")    graph_mean_values->GetYaxis()->SetRangeUser(-0.000005,0.000005); 
    else graph_mean_values->GetYaxis()->SetRangeUser(-0.001,0.001);
    
    std::cout<<" before declaring axes "<<std::endl;
    TAxis *axis = graph_mean_values->GetXaxis();
    axis->Draw();
   
    for(int i=0; i<chamber_name.size(); i++){
       graph_mean_values->GetXaxis()->SetBinLabel(graph_mean_values->GetXaxis()->FindBin(i + 1.), chamber_name[i]); // Find out     which bin on the x-axis the point corresponds to and set the bin label
    }
    graph_mean_values->GetXaxis()->SetTitleOffset(0.1); 
    TCanvas *canv1 = new TCanvas();
    canv1->cd();
    canv1->SetLeftMargin(0.12);
    canv1->SetGrid();
    gPad->SetGrid();
    graph_mean_values->Draw("AP");
    graph_mean_values->SetTitle("");
   
    Double_t *gr_xarray = graph_mean_values->GetX();
    Double_t *gr_yarray = graph_mean_values->GetY();

    std::cout<<" before starting marker "<<std::endl;
    std::map <TString, int> marker_colour = { {"ME11a", 8}, {"ME11b", 8}, 
      {"ME12HV1", 2}, {"ME12HV2", 2}, {"ME12HV3", 2},
      {"ME13HV1", 2}, {"ME13HV2", 2}, {"ME13HV3", 2},
      {"ME21HV1", 4}, {"ME21HV2", 4}, {"ME21HV3", 4},
      {"ME22HV1", 2},{"ME22HV2", 2},{"ME22HV3", 2},{"ME22HV4", 2},{"ME22HV5", 2},
      {"ME31HV1", 4}, {"ME31HV2", 4}, {"ME31HV3", 4},
      {"ME32HV1", 2},{"ME32HV2", 2},{"ME32HV3", 2},{"ME32HV4", 2},{"ME32HV5", 2},
      {"ME41HV1", 4}, {"ME41HV2", 4}, {"ME41HV3", 4},
      {"ME42HV1", 2},{"ME42HV2", 2},{"ME42HV3", 2},{"ME42HV4", 2},{"ME42HV5", 2}
    };
    //int marker_colour[32] = {8, 8, 2, 2, 2, 2,2,2, 4,4,4, 2,2,2,2,2, 4,4,4, 2,2,2,2,2, 4,4,4, 2,2,2,2,2 };
    //int marker_colour[32] = {8, 8, 2, 2, 2, 2,2,2,  4,4,4, 2,2,2,2,2, 4,4,4, 2,2,2,2,2, 4,4,4, 2,2,2,2,2 };
    //int marker_colour[32] = {8, 2, 2, 2, 2, 2,2, 4,4,4, 2,2,2,2,2, 4,4,4, 2,2,2,2,2, 4,4,4, 2,2,2,2,2 };
    TLegend *legend_1 = new TLegend(0.7,0.7,0.9,0.9);
    for (Int_t j=0; j<chamber_name.size(); j++) {
    std::cout<<" declaring gr x arrayr "<<std::endl;
        TMarker *m = new TMarker(gr_xarray[j], gr_yarray[j], 20);
        m->SetMarkerColor(marker_colour[chamber_name[j]]);
        m->Draw();

      std::cout<<"after declaring gr x arrayr "<<std::endl;
			if(j==0) {
				legend_1->AddEntry(m," ME11a, ME11b (10^{0})","p");
			}
			if(j==3) {
			legend_1->AddEntry(m," Outer Chambers (10^{0})","p"); }
			if(j==8)
			legend_1->AddEntry(m," Inner Chambers (20^{0})","p");
   }
   std::cout<<" before drawing mulitgraph "<<std::endl;
	 legend_1->Draw("SAME");

   TLatex* cmslabel_1;
   TLatex* text1,*text2;
   cmslabel_1 = new TLatex(0.18,0.82, "CMS #bf{#it{Preliminary}}");
   cmslabel_1->SetNDC(kTRUE);
   cmslabel_1->SetTextSize(0.06);
   cmslabel_1->SetTextFont(42);
   cmslabel_1->Draw("same");

  if(var=="_integratelumi"){ 
   text1 = new TLatex(0.68,0.91, "#text{pp}, 13#text{TeV}");
   text1->SetNDC(kTRUE);
   text1->SetTextSize(0.06);
   text1->SetTextFont(42);
   text1->Draw("same"); 
  }
  else if(var=="_pressure" || var=="_pressure_second"){
    std::cout<<" entering here : year "<<year<<std::endl;
    if(year=="2016"){
//      text2 = new TLatex(0.46,0.56 , "7000 (#mu b.s)^{-1} #leq L_{inst} #leq 10000 (#mu b.s)^{-1}");
      text2 = new TLatex(0.46,0.56 , cut_string);
//      text1 = new TLatex(0.56,0.91, "pp, L= 40 fb^{-1}(13TeV)");

    }
    else if(year=="2017")  {
      text2 = new TLatex(0.46,0.56 , cut_string);
 //     text1 = new TLatex(0.56,0.91, "pp, L= 44 fb^{-1}(13TeV)");
//      text2 = new TLatex(0.46,0.56 , "7000 (#mu b.s)^{-1} #leq L_{inst} #leq 10000 (#mu b.s)^{-1}");
    }
    else if(year=="2018"){
  //    text1 = new TLatex(0.56,0.91, "pp, L= 65 fb^{-1}(13TeV)");
      text2 = new TLatex(0.46,0.56 , cut_string);
 //     text2 = new TLatex(0.46,0.56 , "10000 (#mu b.s)^{-1} #leq L_{inst} #leq 15000 (#mu b.s)^{-1}");
    }
    else{
   //   text1 = new TLatex(0.56,0.91, "pp, 13TeV");
      text2 = new TLatex(0.46,0.56 , cut_string);
//      text2 = new TLatex(0.56,0.91 , "pp, 13 TeV");
    }

      text2->SetTextFont(42);
      text2->SetTextSize(0.04);
      text2->SetNDC(kTRUE);
      text2->Draw("same");
  
//      text1->SetTextFont(42);
//      text1->SetTextSize(0.06);
//      text1->SetNDC(kTRUE);
//      text1->Draw("same");
    }




	 canv1->SaveAs(result_path+"mean_slope_values_"+var+"_"+year+"_"+fit_name+".pdf"); 
  }
/*	TCanvas *c_2 = new TCanvas("c_2","pressure slope mean vs chamber after second correction",800,800);
	c_2->cd();
	c_2->SetGrid();
	gPad->SetGrid();
	axis_pressure_2->Draw();
	graph_slope_pressure_2->SetMarkerStyle(20);
	graph_slope_pressure_2->SetMarkerSize(1.0);

	graph_slope_pressure_2->Draw("AP");
 
*/
