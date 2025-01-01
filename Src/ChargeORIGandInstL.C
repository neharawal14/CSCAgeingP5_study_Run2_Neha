#include "ChargeORIGandInstL.h"
double instlumi(int runnb, int lumis, TString year){
  TString runnb_tstr = (TString) Form("%d",runnb);
  TString ls_tstr = (TString)Form("%d",lumis);
 
  TString tstrtest = "/cmsuf/data/store/user/t2/users/neha.rawal/CSCAgeing_code/CMSSW_13_3_0/src/files_HVandLumi/InstLumiPerRun/"+year+"/"+runnb_tstr+".csv";
  ifstream lumifile;
  lumifile.open(tstrtest.Data());
  string value;

  runnb_tstr = (TString) Form("%d",runnb)+":";
  ls_tstr = (TString)Form("%d",lumis)+":";

//  std::cout<<" run nb "<<runnb_tstr<<" lumi "<<ls_tstr<<std::endl;
  while ( lumifile.good() )
    {
      getline ( lumifile, value ); 
 
  //std::cout<<" get line "<<value<<std::endl;
      size_t posrunnb = value.find(runnb_tstr.Data() );
      if(posrunnb!=string::npos){
	size_t posLS = value.find( ls_tstr.Data() ,posrunnb+10);
	if(posLS!=string::npos){

	size_t pos2 =  value.find( "STABLE BEAMS");
	pos2 =  value.find( ",",pos2+1);pos2 =  value.find( ",",pos2+1);//pos2 =  value.find( ",",pos2+1);
	string thelumistring = string( value, pos2+1, 8);
//  std::cout<<" lumi string "<<thelumistring<<std::endl;
	double thelumi = stod(thelumistring) ; 
	
	return thelumi/23.31;
	}
      }
    }
  return 0;
}

std::pair<double,double> UncorrGasGain_HVInitial(double charge, int runnb , int stationring, int rhid){
    std::pair<double,double> chargeandHV(0,0);
  double dHV_(0),HV_(0);
  if(stationring ==11 || stationring ==14){

    if( stationring ==14) rhid -=30000 ;
    rhid -= rhid%10; rhid/=10;

    dHV_ = (dHV_HV_ME11(rhid) ).first;
    HV_ = (dHV_HV_ME11(rhid) ).second;
    
  }
  else if(runnb>= 281613){
    dHV_ = (dHV_HV_NonME11_v2(rhid) ).first;
    HV_ = (dHV_HV_NonME11_v2(rhid) ).second;
  } 
  else{
    dHV_ = (dHV_HV_NonME11_v1(rhid) ).first;
    HV_ = (dHV_HV_NonME11_v1(rhid) ).second;
  }  

  // all ME11 set to 2900 V and nonME11 to 3600 V
  if( runnb <277792 ){HV_ -=  dHV_; dHV_=0;}
  
  // all ME11 set to 2900 V , non ME11 have new values
  if( runnb < 281613&& (stationring==11|| stationring ==14)  ){HV_ -=  dHV_; dHV_=0;}

  // all ME11 above 281613 have new values and all non ME11 above 277792 have new values
  else if(runnb >= 324077 && (stationring==11|| stationring ==14))  { HV_= HV_ -32 ; dHV_= dHV_-32; }
  // all ME11 in 2018 now have correct values 
   // Non ME11 outer rings voltage was lowered by 35V
  // ME12, ME13, ME22, ME32, ME42 
  else if(runnb >= 324077 && (stationring==12|| stationring ==13|| stationring ==22|| stationring ==32|| stationring ==42))  { HV_= HV_ -    35 ; dHV_= dHV_-35; }
  // all outer ring chambers have correct values

//  else if(runnb >= 355100 && (stationring==11|| stationring ==14) && (runnb!=360460 || runnb!=360486 || runnb!= 360490 || runnb!=360491))      { HV_ = 2900 ; dHV_ = 0; }
  
  // all rings  ring chambers have correct values
 //Recipe:
 // 
 //   dHV=-ln(G/G0)/B where
 //   G - gas gain in HV segment of the CSC layer (as Trimmed Mean of
 //   distribution SumQ=3X3 ADC
 //   with parameter 0.7)
 //   G0 - reference global gas gain
 //   B - slope (from HV scan data in 2015)
 //   = for inner CSC (ME2/1,ME3/1, ME4/1)     G0=263, B=5.193*10**(-3) 1/V
 //   for outer CSC (ME1/2, ME1/3, ME234/2)  G0=373, B=5.463*10**(-3) 1/V
 //   for ME11: G0= 330, B= 6.262*10**(-3) 1/V
 //   S7 of https://indico.cern.ch/event/555989/contributions/2242293/attachments/1309116/1957949/terentiev_2015_CSC_HV_corrections_talk_v4.pdf
  

  double const_B_ = 0;
  if( (stationring==11|| stationring ==14) ) const_B_ = 6.26e-3;
  else if( (stationring==21|| stationring ==31 || stationring ==41) ) const_B_ = 5.193e-3;
  else  const_B_ = 5.463e-3;

  double chargeold_ = charge * exp(-const_B_* dHV_ ) ;
//  std::cout<<" for the event dHV "<<dHV_<<std::endl;
  chargeandHV.first = chargeold_;
  chargeandHV.second = HV_;
  return chargeandHV ;

}

