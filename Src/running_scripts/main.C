#include "../pressure_dependence_removal_instlumi.h"
int main(int argc, char*argv []){
  for (int i = 0; i < argc; i++) {
     std::cout<<argv[i]<<"\t";
     }
  TString input_file_path = "/eos/home-n/nrawal/CSCAgeing/Run2_combine_new_selections/Run2_combine_updated/";
  TString input_chamber = TString::Format("%s", argv[1]); 
  TString input_file_name = TString::Format("csc_output_run2_"+input_chamber+"_tree_HVupdated.root");
  std::cout<<" input file name "<<input_file_name<<std::endl;

  double intlumi_2016_low , intlumi_2016_up;
  double instlumi_2016_low , instlumi_2016_up;
  double intlumi_2017_low , intlumi_2017_up;
  double instlumi_2017_low , instlumi_2017_up;
  double intlumi_2018_low , intlumi_2018_up;
  double instlumi_2018_low , instlumi_2018_up;

    intlumi_2016_low =5; 
    intlumi_2016_up =18; 
    instlumi_2016_low =6000; 
    instlumi_2016_up =9000; 
    
    intlumi_2017_low =61; 
    intlumi_2017_up =74; 
    instlumi_2017_low =9000; 
    instlumi_2017_up =13000; 
    
    intlumi_2018_low =115; 
    intlumi_2018_up =138; 
    instlumi_2018_low =11000; 
    instlumi_2018_up =15000; 

  TString output_file_path = "/eos/home-n/nrawal/CSCAgeing/2024_new_plots/yearwise_results/Run2_period_new/";
  TString output_path_folder = "./plotfolder/";

  pressure_dependence_removal_instlumi m;

  m.defining_bool(intlumi_2016_low, intlumi_2016_up, instlumi_2016_low, instlumi_2016_up, 
      intlumi_2017_low, intlumi_2017_up, instlumi_2017_low, instlumi_2017_up, 
      intlumi_2018_low, intlumi_2018_up, instlumi_2018_low, instlumi_2018_up
      );
  m.Loop(input_file_path,input_file_name,  input_chamber, output_file_path, output_path_folder);
  std::cout<<" finished the loop "<<std::endl;
  return 0;
}
