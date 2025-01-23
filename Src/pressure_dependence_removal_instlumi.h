//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Jun 15 22:47:51 2017 by ROOT version 6.07/03
// from TTree tree/tree
// found on file: TOTME13HV3.root
//////////////////////////////////////////////////////////

#ifndef pressure_dependence_removal_instlumi_h
#define pressure_dependence_removal_instlumi_h
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH3D.h>
#include <TF1.h>
#include <TGraphErrors.h>
// Header file for the classes stored in the TTree if any.

class pressure_dependence_removal_instlumi {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   ULong64_t       _eventNb;
   ULong64_t       _runNb;
   ULong64_t       _lumiBlock;
   Int_t           _rhid;
   Int_t           _stationring;
   Double_t        _rhsumQ;
   Double_t        _rhsumQ_RAW;
   Double_t        _rhsumQ_equalised_HV_data;
   Double_t        _HV;
   Double_t        _HV_nominal;
   Double_t        z_mass;
   //Double_t        _current;

   Double_t        _pressure;
   Double_t        _temperature;
   Double_t        _instlumi;
   Double_t        _integratelumi;
   UInt_t          _timesecond;
   Int_t           _n_PV;
   Int_t           _bunchcrossing;
   Bool_t          _passZmumusel;

	 // Declaration of leaf types for new tree
   ULong64_t        new_eventNb;
   ULong64_t        new_runNb;
   ULong64_t        new_lumiBlock;
   Int_t            new_rhid;
   Int_t            new_stationring;
   Double_t         new_rhsumQ;
   Double_t         new_rhsumQ_RAW;
   Double_t         new_rhsumQ_equalised_HV_data;
   Double_t         new_HV;
   Double_t         new_HV_nominal;
   //Double_t         new_current;

   Double_t         new_pressure;
   Double_t         new_temperature;
   Double_t         new_instlumi;
   Double_t         new_integratelumi;
   UInt_t           new_timesecond;
   Int_t            new_n_PV;
   Int_t            new_bunchcrossing;
   Double_t         new_z_mass;

   Bool_t          new_passZmumusel;

   // List of branches
   TBranch        *b__eventNb;   //!
   TBranch        *b__runNb;   //!
   TBranch        *b__lumiBlock;   //!
   TBranch        *b__rhid;   //!
   TBranch        *b__stationring;   //!
   TBranch        *b__rhsumQ;   //!
   TBranch        *b__rhsumQ_RAW;   //!
   TBranch        *b__HV;   //!
   TBranch        *b_z_mass;   //!
//   TBranch        *b__current;   //!
   TBranch        *b__pressure;   //!
   TBranch        *b__temperature;   //!
   TBranch        *b__instlumi;   //!
   TBranch        *b__integratelumi;   //!
   TBranch        *b__timesecond;   //!
   TBranch        *b__n_PV;   //!
   TBranch        *b__bunchcrossing;   //!
   TBranch        *b__passZmumusel;   //!
   TBranch        *b__rhsumQ_equalised_HV_data;
   TBranch        *b__HV_nominal;


	 TDirectory *dir_var_name;
   pressure_dependence_removal_instlumi(TTree *tree=0);
   virtual ~pressure_dependence_removal_instlumi();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);



	std::vector<float > trimmed_mean(TH2D *myh);
   std::vector <std::pair<double,double > >  GetSlope( TH3D * myh , TString thevar , TString filename, TString title, TFile * outf, TString chamber_name_string);
/*   void defining_bool(double, double, double ,double,
       double, double, double, double, 
       double, double, double, double); */
   void defining_bool(TString , double, double, double ,double);
   //void defining_bool();

   double ApplyCorrection( double pressure ,TString correctiontype, double p0, double p1 );
   virtual void  Loop(TString , TString, TString, TString, TString);
   
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);

	  TTree *tree_new;
	  TString chamber_string_name; 
    TString output_path ;
    TString output_plots_folder ; 
		TString detregionstr;

/*		std::vector<UInt_t> *time_vector=nullptr;
		std::vector<double> *gas_gain_vector=nullptr;
		std::vector<double> *gas_gain_error_vector=nullptr;
*/
		std::vector<UInt_t> time_vector;
		std::vector<double> gas_gain_vector;
		std::vector<double> gas_gain_error_vector;
		double time_value;
		double old_time;
    std::string new_time;
    const char* new_time_Char;

	  std::string time_conv(double timeSeconds);
		void plot_gain_time();
    void Setup_new_tree();

    double NominalHV();

    TString year;
      bool testing_code ;
      bool second_iteration;
      // for intlumi corrections, time
      bool intlumi_initial;
      bool time_initial;
      bool time_final ;
      bool intlumi_final ;
      
      // for anlaysing only 2016
      bool intlumi_corr_2016 ;
      bool intlumi_corr_2017 ;
      bool intlumi_corr_2018 ;
      
      // for pressure corrections
      // flags used : pressure_corr, pressure_corr_2016, pressure_corr_2017, pressure_corr_2018, intlumi_instlumi_const
      bool pressure_corr ;
      bool pressure_corr_2016 ;
      bool pressure_corr_2017 ;
      bool pressure_corr_2018 ;
      bool intlumi_instlumi_const ;
      // instlumi corrections
      bool instlumi_corr ;
      bool instlumi_corr_2016 ;
      bool instlumi_corr_2017 ;
      bool instlumi_corr_2018 ;
      bool intlumi_const ;
      bool pressure_const ;

     double instlumi_low_cut_2016, instlumi_up_cut_2016; 
     double instlumi_low_cut_2017, instlumi_up_cut_2017; 
     double instlumi_low_cut_2018, instlumi_up_cut_2018; 
     double intlumi_low_cut_2016, intlumi_up_cut_2016; 
     double intlumi_low_cut_2017, intlumi_up_cut_2017; 
     double intlumi_low_cut_2018, intlumi_up_cut_2018; 

};

#endif

//#ifdef pressure_dependence_removal_instlumi_cxx
inline pressure_dependence_removal_instlumi::pressure_dependence_removal_instlumi(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
 
}

inline pressure_dependence_removal_instlumi::~pressure_dependence_removal_instlumi()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
   fChain = nullptr;
}

inline Int_t pressure_dependence_removal_instlumi::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
inline Long64_t pressure_dependence_removal_instlumi::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}


inline void pressure_dependence_removal_instlumi::Setup_new_tree(){

 tree_new->Branch("_passZmumusel",&new_passZmumusel, "new_passZmumusel/O");
 tree_new->Branch("_eventNb",&new_eventNb, "new_eventNb/l");
 tree_new->Branch("z_mass",&new_z_mass, "new_z_mass/D");
 tree_new->Branch("_runNb",&new_runNb, "new_runNb/l");
 tree_new->Branch("_lumiBlock",&new_lumiBlock, "new_lumiBlock/l");
 tree_new->Branch("_rhid",&new_rhid, "new_rhid/I");
 tree_new->Branch("_stationring",&new_stationring, "new_stationring/I");
 tree_new->Branch("_rhsumQ",&new_rhsumQ , "new_rhsumQ/D");
 tree_new->Branch("_rhsumQ_RAW", &new_rhsumQ_RAW, "new_rhsumQ_RAW/D");
 tree_new->Branch("_HV", &new_HV, "new_HV/D");
// tree_new->Branch("_current", &new_current, "new_current/D");
 tree_new->Branch("_pressure", &new_pressure, "new_pressure/D");
 tree_new->Branch("_temperature", &new_temperature, "new_temperature/D");
 tree_new->Branch("_instlumi", &new_instlumi, "new_instlumi/D");
 tree_new->Branch("_integratelumi", &new_integratelumi, "new_integratelumi/D");
 tree_new->Branch("_timesecond",&new_timesecond, "new_timesecond/i");
 tree_new->Branch("_n_PV", &new_n_PV, "new_n_PV/I");
 tree_new->Branch("_bunchcrossing", &new_bunchcrossing, "new_bunchcrossing/I");

}

inline void pressure_dependence_removal_instlumi::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("_passZmumusel", &_passZmumusel, &b__passZmumusel);
   fChain->SetBranchAddress("_eventNb", &_eventNb, &b__eventNb);
   fChain->SetBranchAddress("z_mass", &z_mass, &b_z_mass);
   fChain->SetBranchAddress("_runNb", &_runNb, &b__runNb);
   fChain->SetBranchAddress("_lumiBlock", &_lumiBlock, &b__lumiBlock);
   fChain->SetBranchAddress("_rhid", &_rhid, &b__rhid);
   fChain->SetBranchAddress("_stationring", &_stationring, &b__stationring);
   fChain->SetBranchAddress("_rhsumQ", &_rhsumQ, &b__rhsumQ);
   fChain->SetBranchAddress("_rhsumQ_RAW", &_rhsumQ_RAW, &b__rhsumQ_RAW);
   fChain->SetBranchAddress("_rhsumQ_equalised_HV_data", &_rhsumQ_equalised_HV_data, &b__rhsumQ_equalised_HV_data);
   fChain->SetBranchAddress("_HV", &_HV, &b__HV);
   fChain->SetBranchAddress("_HV_nominal", &_HV_nominal, &b__HV_nominal);
//   fChain->SetBranchAddress("_current", &_current, &b__current);
   fChain->SetBranchAddress("_pressure", &_pressure, &b__pressure);
   fChain->SetBranchAddress("_temperature", &_temperature, &b__temperature);
   fChain->SetBranchAddress("_instlumi", &_instlumi, &b__instlumi);
   fChain->SetBranchAddress("_integratelumi", &_integratelumi, &b__integratelumi);
   fChain->SetBranchAddress("_timesecond", &_timesecond, &b__timesecond);
//   fChain->SetBranchAddress("_n_PV", &_n_PV, &b__n_PV);
   fChain->SetBranchAddress("_bunchcrossing", &_bunchcrossing, &b__bunchcrossing);

   fChain->SetBranchStatus("_temperature",0);
   fChain->SetBranchStatus("_lumiBlock",0);
   fChain->SetBranchStatus("_eventNb",0);
//   fChain->SetBranchStatus("_n_PV",0);
   fChain->SetBranchStatus("_bunchcrossing",0);
//   fChain->SetBranchStatus("_timesecond",0);

   Notify();
   
}
//std::string time_conv(double timeSeconds) {
//    // Set the time in seconds                                                                                                       
//                                                                                                                                     
//    // Convert to time_t (seconds since epoch)                                                                                       
//    time_t timeEpoch = timeSeconds;
//                                                                                                                                     
//    // Convert to tm structure                                                                                                       
//    struct tm* timeInfo = gmtime(&timeEpoch);
//                                                                                                                                     
//    // Format the date and time                                                                                                      
//    std::string strBuffer(80, '\0'); 
//    strftime(&strBuffer[0], 80, "%d/%m", timeInfo);
//                                                                                                                                     
//    // Print the date and time                                                                                                       
//    std::cout << "Time in seconds: " << timeSeconds << std::endl; 
//    std::cout << "Date and time: " << strBuffer << std::endl;
//                                                                                                                                     
//    return strBuffer;
//}

inline Bool_t pressure_dependence_removal_instlumi::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

inline void pressure_dependence_removal_instlumi::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
inline Int_t pressure_dependence_removal_instlumi::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}


//#endif // #ifdef pressure_dependence_removal_instlumi_cxx
