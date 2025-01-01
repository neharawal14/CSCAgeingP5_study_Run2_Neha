#include "../pressure_dependence_removal_instlumi.h"
int main(int argc, char*argv []){
  TString year = "2018";
  for (int i = 0; i < argc; i++) {
     std::cout<<argv[i]<<"\t";
     }
  TString input_file_path = "/eos/home-n/nrawal/CSCAgeing/Run2_combine_new_selections/"+year+"_updated/";
  TString input_chamber = TString::Format("%s", argv[1]); 
  TString number = TString::Format("%s", argv[2]); 
  TString input_file_name = TString::Format("csc_output_"+year+"_"+input_chamber+"_tree_HVupdated.root");
  std::cout<<" input file name "<<input_file_name<<std::endl;
  std::cout<<" number :"<<number<<std::endl;

  double intlumi_low , intlumi_up;
  double instlumi_low , instlumi_up;

  if(number=="first"){
    intlumi_low =85; 
    intlumi_up =108; 
   instlumi_low =11000; 
    instlumi_up =15000; 

//    instlumi_low =0; 
//    instlumi_up =30000; 
  }
  if(number=="second"){
    intlumi_low =115; 
    intlumi_up =138; 
    instlumi_low =11000; 
    instlumi_up =15000; 
//    instlumi_low =0; 
//    instlumi_up =30000; 

  }
  if(number=="third"){
    intlumi_low =140; 
    intlumi_up =150; 
    instlumi_low =11000; 
    instlumi_up =15000; 
//    instlumi_low =0; 
//    instlumi_up =30000; 

  }
  if(number=="complete"){
    intlumi_low =85; 
    intlumi_up =150; 
    instlumi_low =11000; 
    instlumi_up =15000; 
//    instlumi_low =0; 
//    instlumi_up =30000; 

  }


  TString output_file_path = "/eos/home-n/nrawal/CSCAgeing/2024_new_plots/yearwise_results/new_plots_fixed/"+year+"_"+number+"_period/";
  TString output_path_folder = "./plotfolder/";

  pressure_dependence_removal_instlumi m;

  m.defining_bool(year, intlumi_low, intlumi_up, instlumi_low, instlumi_up);
  m.Loop(input_file_path,input_file_name,  input_chamber, output_file_path, output_path_folder);
  std::cout<<" finished the loop "<<std::endl;
  return 0;
}
