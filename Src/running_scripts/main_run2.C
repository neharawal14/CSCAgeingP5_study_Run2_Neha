#include "../pressure_dependence_removal_instlumi.h"
int main(int argc, char*argv []){
//int main(){
  for (int i = 0; i < argc; i++) {
     std::cout<<argv[i]<<"\t";
     }
   // Convert the first argument to an integer

  //TString input_file_path = "/eos/home-n/nrawal/CSCAgeing/Run2_combine/";
  //TString input_file_path = "/eos/home-n/nrawal/CSCAgeing/Run2_combine_new_selections/Run2_combine_updated/";
  TString input_file_path = "/eos/home-n/nrawal/CSCAgeing/Run2_combine_new_selections/Run2_combine_updated/";

  TString input_chamber = TString::Format("%s", argv[1]); 
  TString input_file_name = TString::Format("csc_output_run2_"+input_chamber+"_tree.root");
  std::cout<<" input file name "<<input_file_name<<std::endl;

  TString output_file_path = "/eos/home-n/nrawal/CSCAgeing/2024_new_plots/Removed_HV_trips/Reproduced/Run2_results_wider_new/";
  TString output_path_folder = "./plotfolder/";

pressure_dependence_removal_instlumi m;
m.defining_bool();
m.Loop(input_file_path,input_file_name,  input_chamber, output_file_path, output_path_folder);
std::cout<<" finished the loop "<<std::endl;
return 0;
}

