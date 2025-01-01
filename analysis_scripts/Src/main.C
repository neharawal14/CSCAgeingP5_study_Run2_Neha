#include "CumulativePlots.h"
#include "individual_channels.h"
#include "MeanPlots.h"
#include "SystematicRemoval.h"
int main(int argc, char *argv[]){
  bool second_iteration=true;
  std::vector<TString> chamber ;
  TString year = TString::Format("%s", argv[1]);
  TString number = TString::Format("%s", argv[2]);
  TString input_path =  TString::Format("%s",argv[3]);
  TString my_output_path =  TString::Format("%s",argv[4]);

  for(int i=4; i<argc-1 ; i++){
    TString chamber_name = TString::Format("%s", argv[i+1]);
    std::cout<<" arg "<<chamber_name<<std::endl;	
    chamber.push_back(chamber_name);
  }
  CumulativePlot plot_obj;
  CumulativePlot plot_obj_second;
  individual_channels plot_obj_indie;
	std::cout<<" started program"<<std::endl;
  std::pair<float, float> mean_values_pressure_pair;
  std::pair<float, float> mean_values_instlumi_pair;
  std::pair<float, float> mean_values_pressure_pair_fit;
  std::pair<float, float> mean_values_instlumi_pair_fit;

  std::vector<std::pair<float, float>> mean_values_pressure;
  std::vector<std::pair<float, float>> mean_values_instlumi;
  std::vector<std::pair<float, float>> mean_values_pressure_fit;
  std::vector<std::pair<float, float>> mean_values_instlumi_fit;
	
  std::pair<float, float> mean_values_pressure_second_pair;
  std::pair<float, float> mean_values_instlumi_second_pair;
  std::pair<float, float> mean_values_pressure_second_pair_fit;
  std::pair<float, float> mean_values_instlumi_second_pair_fit;

  std::vector<std::pair<float, float>> mean_values_pressure_second;
  std::vector<std::pair<float, float>> mean_values_instlumi_second;
  std::vector<std::pair<float, float>> mean_values_pressure_second_fit;
  std::vector<std::pair<float, float>> mean_values_instlumi_second_fit;

  TString saving_path = my_output_path+"/cumulative_plots/";
	for(int i=0; i<chamber.size();i++){
    	std::cout<<" about to initialise the chamber progra:"<<std::endl;
      TString input_file_name = input_path+"outf_dataset_pressure_corrected__"+chamber[i]+"_output_run2_const.root";
    	
      //TString saving_path_indie = "../results/"+year+"_"+number+"_period/cumulative_plots/individual_channel/";
    	std::cout<<" about to initialise the chamber progra:"<<std::endl;
  	  plot_obj.initialise(chamber[i], input_file_name, saving_path);
  	  //plot_obj_indie.initialise(chamber[i], input_file_name, saving_path_indie);
    	std::cout<<" started progra:"<<std::endl;
      TString pressure_string = "_pressure_"+year;
      TString instlumi_string = "_instlumi_"+year;
      TString intlumi_string = "_integratelumi_initial";
      //mean_values_pressure_pair_fit = plot_obj_indie.plot_individual_channels(pressure_string);
      //mean_values_instlumi_pair_fit = plot_obj_indie.plot_individual_channels(instlumi_string);
    	std::cout<<" done with individual channels "<<std::endl;
    	plot_obj.plot_goodchannels(pressure_string);
    	std::cout<<" started progra:"<<chamber[i]<<std::endl;

    	plot_obj.plot_goodchannels(instlumi_string);
    	mean_values_pressure_pair = plot_obj.fit_goodchannels(pressure_string);
    	mean_values_instlumi_pair = plot_obj.fit_goodchannels(instlumi_string);

      mean_values_pressure.push_back(mean_values_pressure_pair);
      mean_values_instlumi.push_back(mean_values_instlumi_pair);
//      mean_values_pressure_fit.push_back(mean_values_pressure_pair_fit);
//      mean_values_instlumi_fit.push_back(mean_values_instlumi_pair_fit);

      std::cout<<" started progra:"<<std::endl;
      if(second_iteration){
  	  plot_obj_second.initialise(chamber[i], input_file_name, saving_path);
      pressure_string = "_pressure_"+year+"_second";
      instlumi_string = "_instlumi_"+year+"_second";
//      mean_values_pressure_second_pair_fit = plot_obj_indie.plot_individual_channels(pressure_string);
//      mean_values_instlumi_second_pair_fit = plot_obj_indie.plot_individual_channels(instlumi_string);
    	std::cout<<" done with individual channels "<<std::endl;
    	plot_obj_second.plot_goodchannels(pressure_string);
    	plot_obj_second.plot_goodchannels(instlumi_string);
    	std::cout<<" started progra:"<<chamber[i]<<std::endl;

    	mean_values_pressure_second_pair = plot_obj.fit_goodchannels(pressure_string);
    	mean_values_instlumi_second_pair = plot_obj.fit_goodchannels(instlumi_string);

      mean_values_pressure_second.push_back(mean_values_pressure_second_pair);
      mean_values_instlumi_second.push_back(mean_values_instlumi_second_pair);
//      mean_values_pressure_second_fit.push_back(mean_values_pressure_second_pair_fit);
//      mean_values_instlumi_second_fit.push_back(mean_values_instlumi_second_pair_fit);

      }  
  	  plot_obj.plot_goodchannels("_integratelumi_initial");
   	  plot_obj.plot_goodchannels("_integratelumi");
 }
  TString  cut_string = plot_obj.get_string();
  TString mean_save_path = my_output_path+"MeanPlots/";
  //TString mean_save_path = "../results/"+year+"_"+number+"_period/MeanPlots/";

 // after obtaining the mean of slope for all the respective pressure or int lumi files 
 // Lets make the plot for mean of slope vs chamber 
 // First for pressure
///  std::cout<<" will plot mean values now on  canvas"<<std::endl;
///  for(int i=0;i<chamber.size(); i++){
///      std::cout<<" mean :  chamber "<<chamber[i]<<" : "<<mean_values_pressure[i].first<<" error "<<mean_values_pressure[i].second<<std::endl;
///      //std::cout<<" mean :  chamber "<<chamber[i]<<" : "<<mean_values_pressure_2017[i].first<<std::endl;
///  }
	MeanPlot mean_plot_pressure;
	MeanPlot mean_plot_instlumi;
//	MeanPlot mean_plot_pressure_fit;
//	MeanPlot mean_plot_instlumi_fit;

  std::cout<<" initialising"<<std::endl;
	mean_plot_pressure.initialise(mean_values_pressure, chamber, year, "_pressure","direct", mean_save_path, cut_string); 
	mean_plot_instlumi.initialise(mean_values_instlumi, chamber, year,"_instlumi", "direct", mean_save_path, cut_string); 
  std::cout<<" mean values "<<std::endl;

	mean_plot_pressure.plot_mean();
	mean_plot_instlumi.plot_mean();
  std::cout<<" plotting fits"<<std::endl;
//  mean_plot_pressure_fit.initialise(mean_values_pressure_fit, chamber, year, "_pressure", "fit", mean_save_path, cut_string); 
  std::cout<<" mean values "<<std::endl;
//	mean_plot_instlumi_fit.initialise(mean_values_instlumi_fit, chamber, year,"_instlumi", "fit", mean_save_path, cut_string); 

//	mean_plot_pressure_fit.plot_mean();
//	mean_plot_instlumi_fit.plot_mean();

  // SEcond iteration
  //
  if(second_iteration){
  MeanPlot mean_plot_pressure_second;
  MeanPlot mean_plot_instlumi_second;
//  MeanPlot mean_plot_pressure_second_fit;
//  MeanPlot mean_plot_instlumi_second_fit;

  std::cout<<" initialising"<<std::endl;
  mean_plot_pressure_second.initialise(mean_values_pressure_second, chamber, year, "_pressure_second","direct_second", mean_save_path, cut_string); 
  mean_plot_instlumi_second.initialise(mean_values_instlumi_second, chamber, year,"_instlumi_second", "direct_second", mean_save_path, cut_string); 
  std::cout<<" mean values "<<std::endl;

  mean_plot_pressure_second.plot_mean();
  mean_plot_instlumi_second.plot_mean();
  std::cout<<" plotting fits"<<std::endl;
//  mean_plot_pressure_second_fit.initialise(mean_values_pressure_second_fit, chamber, year, "_pressure_second", "fit_second", mean_save_path, cut_string); 
  std::cout<<" mean values "<<std::endl;
//  mean_plot_instlumi_second_fit.initialise(mean_values_instlumi_second_fit, chamber, year,"_instlumi_second", "fit_second"); 

//  mean_plot_pressure_second_fit.plot_mean();
//  mean_plot_instlumi_second_fit.plot_mean();
  }

  std::cout<<" here the normalisation "<<std::endl;
  std::pair<float, float> mean_values_integratelumi_value;
  std::vector<std::pair<float, float> > mean_values_integratelumi;
  TString sys_saving_path =  my_output_path+"/systematic_normalised_plots/";
  SystematicRemoval sys_obj;
  std::cout<<" declared object "<<std::endl;
  sys_obj.initialise(input_path, sys_saving_path, "ME13HV3");
  std::cout<<" initialised  object "<<std::endl;
  for(int i=0; i<chamber.size(); i++){

    TString input_file_name = input_path+"outf_dataset_pressure_corrected__"+chamber[i]+"_output_run2_const.root";
  std::cout<<" input file "<<std::endl;
    sys_obj.adding_plus_minus(chamber[i]);
    std::cout<<" added now going to ratio "<<std::endl;
    sys_obj.ratio(chamber[i]);
    std::cout<<" done ratio ones  values "<<std::endl;
    mean_values_integratelumi_value = sys_obj.normalised_fit(chamber[i]);
    std::cout<<" mean values "<<mean_values_integratelumi_value.first<<std::endl;
    mean_values_integratelumi.push_back(mean_values_integratelumi_value);
 }
	MeanPlot mean_plot_intlumi;
  std::cout<<" initiaiseing mean plot intlumi"<<std::endl;
  //TString mean_save_path = my_output_path+"MeanPlots/";
	//mean_plot_intlumi.initialise(mean_values_integratelumi, chamber, year, "_integratelumi", "direct",mean_save_path, cut_string); 
	mean_plot_intlumi.initialise(mean_values_integratelumi, chamber, year, "_integratelumi", "direct",mean_save_path, cut_string); 
	mean_plot_intlumi.plot_mean();

	std::cout<<" started program"<<std::endl;

return 0;
}
