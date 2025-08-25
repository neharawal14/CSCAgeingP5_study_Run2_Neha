#include "HistMan.h"
#include "AnalysisGasGain.h"
#include "pressurecsc_2016.h"
#include "IntegrateLumi_2016.h"
#include "pressurecsc_2017.h"
#include "IntegrateLumi_2017.h"
#include "pressurecsc_2018.h"
#include "IntegrateLumi_2018.h"
#include "ChargeORIGandInstL.h"
ClassImp(AnalysisGasGain)

std::map <Int_t, Int_t>     m_Run_usedevents,m_Run_usedhits;
std::map <Int_t, Int_t>     m_RunEvent;
std::map <Int_t, UInt_t>    m_RunStart;
std::map <Int_t, UInt_t>    m_RunEnd;
std::map <Int_t, Int_t>     m_ordstatring;
std::map <Int_t, Int_t>     m_nRecHitlayer;
std::map <Int_t, Int_t>     m_nRecHitchamber;
std::map <Int_t, Int_t>     m_nRecHitlayerME14;
std::map <Int_t, std::vector <Double_t> > 
                            m_RecHitlayerME14_final;
std::map <UInt_t, std::vector <Double_t> >  
                            m_cscSegments_recHitRecordX,
                            m_cscSegments_recHitRecordY;
std::map <Int_t, Int_t>     m_nsegments_chamber;
std::map <UInt_t, std::vector <Double_t> >  
                            m_muon_segm;
std::map <Int_t, std::vector <Double_t> >  
                            m_Single_cscSegments_recHitRecordX,
                            m_Single_cscSegments_recHitRecordY;
//std::vector<std::map <Int_t, std::vector <Double_t> >>  
//                            m_Single_cscSegments_recHitRecordX,
//                            m_Single_cscSegments_recHitRecordY;

std::map <Int_t, std::vector <Double_t> >  
                            m_cscSegments_single_trk_recHitRecord,
                            m_cscSegments_single_trk_recHitRecord_final;

int minhitpersegment = 5;
bool debug_new = false;
bool debug_bool = false;
bool debug_bool_region = false;
bool debug_program = false;
// Important note : Endcap =1 => Plus Endcap , Endcap =2 => Minus Endcap
AnalysisGasGain::AnalysisGasGain() { }

AnalysisGasGain::~AnalysisGasGain() { }

/* **************************** Setup ************************************* */

void AnalysisGasGain::Setup(Int_t fstat,Int_t fprint,string inp,string out, string year_string)
{
  flag_stat=fstat;
  flag_print=fprint;
  ntuplename=inp;
  histrootname=out;
	year = year_string;
  if(debug_bool) std::cout<<"starting setup"<<std::endl;
  Int_t strng[10]={11,12,13,14,21,22,31,32,41,42};
  m_ordstatring.clear();
  for(Int_t i=0;i<10;i++) {
    Int_t key=strng[i];
    if(m_ordstatring.find(key)==m_ordstatring.end())
      m_ordstatring[key]=i+1; // just for order numeration to use in hists
  }
 
 for(Int_t i=0;i<3;i++) zero3.push_back(0.0);
 for(Int_t i=0;i<6;i++) zero6.push_back(-999.0);
 for(Int_t i=0;i<5;i++) zero5.push_back(0.0);

 // Calculate and store Yloc boundaries of the HV segments in CSC
 //**************************************************
 //CSC        ME12   ME13   ME21  ME31  ME41  ME234/2 
 //# HV segm   3      3      3     3     3     5
 //**************************************************

 const Int_t ntype=6;
 const Int_t nsegm3=3;
 const Int_t nsegm5=5;

 // Al frame height, (cm, between outer edges)
 Float_t h[ntype]={189.4, 179.3, 204.6, 184.6, 164.7, 338.0};
 // dFtoAP-distance from the narrow edge of the Al Frame to the Alignment Pin
 Float_t dftoap=3.49; // cm
 // Distances (mm) of the first (*Low) and last (*High) thick wires 
 // of the HV segment from alignment pin 
 Float_t dapme12Low[nsegm3] = { 28.5,  594.3, 1251.8},   
         dapme12High[nsegm3]= {572.2, 1229.7, 1776.6},

	 dapme13Low[nsegm3] = { 28.5,  645.3, 1151.3},   
	 dapme13High[nsegm3]= {623.1, 1129.2, 1673.2}, 

	 dapme21Low[nsegm3] = { 28.7,  724.3, 1335.7},   
	 dapme21High[nsegm3]= {702.5, 1313.9, 1928.4}, 

	 dapme31Low[nsegm3] = { 28.4,  536.9, 1135.8},   
	 dapme31High[nsegm3]= {515.0, 1114.0, 1728.5},

	 dapme41Low[nsegm3] = { 30.4,  538.9, 1038.0},   
	 dapme41High[nsegm3]= {517.0, 1016.1, 1527.7},

	 dapme234_2Low[nsegm5]={ 28.7,   847.4, 1454.3, 2061.3, 2668.2},
	 dapme234_2High[nsegm5]={825.3, 1432.2, 2039.1, 2646.1, 3262.5};

 // Calculate Y loc for HV segment boundaries (Low and High).
 // Y loc = 0 is in the symmetry center of Al frame, e.g. at h[]/2.0
 for(Int_t i=0;i<nsegm3;i++) {
   me12YlocHVsgmLow[i]    = -h[0]/2.0+dftoap+dapme12Low[i]/10.0;
   me12YlocHVsgmHigh[i]   = -h[0]/2.0+dftoap+dapme12High[i]/10.0; 

   me13YlocHVsgmLow[i]    = -h[1]/2.0+dftoap+dapme13Low[i]/10.0;
   me13YlocHVsgmHigh[i]   = -h[1]/2.0+dftoap+dapme13High[i]/10.0;

   me21YlocHVsgmLow[i]    = -h[2]/2.0+dftoap+dapme21Low[i]/10.0;
   me21YlocHVsgmHigh[i]   = -h[2]/2.0+dftoap+dapme21High[i]/10.0;

   me31YlocHVsgmLow[i]    = -h[3]/2.0+dftoap+dapme31Low[i]/10.0;
   me31YlocHVsgmHigh[i]   = -h[3]/2.0+dftoap+dapme31High[i]/10.0; 
 
   me41YlocHVsgmLow[i]    = -h[4]/2.0+dftoap+dapme41Low[i]/10.0;
   me41YlocHVsgmHigh[i]   = -h[4]/2.0+dftoap+dapme41High[i]/10.0; 
 }
 for(Int_t i=0;i<nsegm5;i++) {
   me234_2YlocHVsgmLow[i]    = -h[5]/2.0+dftoap+dapme234_2Low[i]/10.0;
   me234_2YlocHVsgmHigh[i]   = -h[5]/2.0+dftoap+dapme234_2High[i]/10.0; 
 }
  // end of Y loc HV segment boundaries calculations
	if(debug_bool) std::cout<<"Setup before  "<<std::endl;
} // End of Setup function

void AnalysisGasGain::SetupTree(){
  //this->SetupPrint();
  TString filetreelocation1 = (TString) histrootname;
  filetreelocation1.ReplaceAll(".root","_tree.root");
  myoutfilefortree1 = new TFile(filetreelocation1,"RECREATE"); 
	if(debug_bool) std::cout<<" trees created for all the segments"<<std::endl;
  // The new tree should save the following information
  // One tree for all the hits in any segment and then 32 files according to the segment the hits belongs to
  outputtree1= new TTree("tree","tree");
  outputtree1->Branch("_passZmumusel",   &passZmumusel,   "_passZmumusel/O");
  outputtree1->Branch("_passisomuondzdxy",   &passisomuondzdxy,   "_passisomuondzdxy/O");
  outputtree1->Branch("_eventNb",   &_eventNb,   "_eventNb/l");
  outputtree1->Branch("_runNb",   &_runNb,   "_runNb/l");
  outputtree1->Branch("_lumiBlock",   &_lumiBlock,   "_lumiBlock/l");
  outputtree1->Branch("_rhid",&_rhid,"_rhid/I");
  outputtree1->Branch("_stationring",&_stationring,"_stationring/I");
  outputtree1->Branch("_nearestStrip",&_nearestStrip,"_nearestStrip/I");
  outputtree1->Branch("_nearestWireGroup",&_nearestWireGroup,"_nearestWireGroup/I");
  outputtree1->Branch("_rhsumQ",&_rhsumQ,"_rhsumQ/D");
  outputtree1->Branch("_rhsumQ_RAW",&_rhsumQ_RAW,"_rhsumQ_RAW/D");
  outputtree1->Branch("_HV",&_HV,"_HV/D");
  outputtree1->Branch("_current",&_current,"_current/D");
  outputtree1->Branch("_pressure",&_pressure,"_pressure/D");
  outputtree1->Branch("_temperature",&_temperature,"_temperature/D");
  outputtree1->Branch("_instlumi",&_instlumi,"_instlumi/D");
  outputtree1->Branch("_integratelumi",&_integratelumi,"_integratelumi/D");
  outputtree1->Branch("_timesecond",&_timesecond,"_timesecond/i") ;
  outputtree1->Branch("_n_PV",&_n_PV,"_n_PV/I");
  outputtree1->Branch("_bunchcrossing",&_bunchcrossing,"_bunchcrossing/I");
  outputtree1->Branch("_etamuon",&_etamuon,"_etamuon/D");
  outputtree1->Branch("_phimuon",&_phimuon,"_phimuon/D");
  outputtree1->Branch("_ptmuon",&_ptmuon,"_ptmuon/D");
	outputtree1->Branch("iso_PF_first",&iso_PF_first,"iso_PF_first/D");
	outputtree1->Branch("iso_PF_second",&iso_PF_second,"iso_PF_second/D");
	outputtree1->Branch("z_pt",&z_pt,"z_pt/D");
  outputtree1->Branch("z_eta",&z_eta,"z_eta/D");
  outputtree1->Branch("z_phi",&z_phi,"z_phi/D");
  outputtree1->Branch("z_mass",&z_mass,"z_mass/D");
  outputtree1->Branch("isolation1",&isolation1,"isolation1/D");
  outputtree1->Branch("isolation2",&isolation2,"isolation2/D");
  for(int i = 0; i <32;i++){
		if(debug_bool) std::cout<<"creating trees for all the segments"<<std::endl;
    TString treename =GetRegionName(i);
    TString filetreelocation = (TString) histrootname;
    filetreelocation.ReplaceAll(".root",treename+"_tree.root");
    myoutfilefortree[i] = new TFile(filetreelocation,"RECREATE"); 
    
    outputtree[i] = new TTree("tree","tree");
    outputtree[i]->Branch("_passZmumusel",   &passZmumusel,   "_passZmumusel/O");
    outputtree[i]->Branch("_passisomuondzdxy",   &passisomuondzdxy,   "_passisomuondzdxy/O");
    outputtree[i]->Branch("_eventNb",   &_eventNb,   "_eventNb/l");
    outputtree[i]->Branch("_runNb",   &_runNb,   "_runNb/l");
    outputtree[i]->Branch("_lumiBlock",   &_lumiBlock,   "_lumiBlock/l");
    outputtree[i]->Branch("_rhid",&_rhid,"_rhid/I");
    outputtree[i]->Branch("_stationring",&_stationring,"_stationring/I");
    outputtree[i]->Branch("_nearestStrip",&_nearestStrip,"_nearestStrip/I");
    outputtree[i]->Branch("_nearestWireGroup",&_nearestWireGroup,"_nearestWireGroup/I");
    outputtree[i]->Branch("_rhsumQ",&_rhsumQ,"_rhsumQ/D");
    outputtree[i]->Branch("_rhsumQ_RAW",&_rhsumQ_RAW,"_rhsumQ_RAW/D");
    outputtree[i]->Branch("_HV",&_HV,"_HV/D");
    outputtree[i]->Branch("_current",&_current,"_current/D");
    outputtree[i]->Branch("_pressure",&_pressure,"_pressure/D");
    outputtree[i]->Branch("_temperature",&_temperature,"_temperature/D");
    outputtree[i]->Branch("_instlumi",&_instlumi,"_instlumi/D");
    outputtree[i]->Branch("_integratelumi",&_integratelumi,"_integratelumi/D");
    outputtree[i]->Branch("_timesecond",&_timesecond,"_timesecond/i") ;
    outputtree[i]->Branch("_n_PV",&_n_PV,"_n_PV/I");
    outputtree[i]->Branch("_bunchcrossing",&_bunchcrossing,"_bunchcrossing/I");
    outputtree[i]->Branch("_etamuon",&_etamuon,"_etamuon/D");
    outputtree[i]->Branch("_phimuon",&_phimuon,"_phimuon/D");
    outputtree[i]->Branch("_ptmuon",&_ptmuon,"_ptmuon/D");
	  outputtree[i]->Branch("iso_PF_first",&iso_PF_first,"iso_PF_first/D");
	  outputtree[i]->Branch("iso_PF_second",&iso_PF_second,"iso_PF_second/D");
	  outputtree[i]->Branch("z_pt",&z_pt,"z_pt/D");
    outputtree[i]->Branch("z_eta",&z_eta,"z_eta/D");
    outputtree[i]->Branch("z_phi",&z_phi,"z_phi/D");
    outputtree[i]->Branch("z_mass",&z_mass,"z_mass/D");
    outputtree[i]->Branch("isolation1",&isolation1,"isolation1/D");
    outputtree[i]->Branch("isolation2",&isolation2,"isolation2/D");
		if(debug_bool) std::cout<<"Setup perfectly "<<std::endl;
  }
}// end of SetupTree

/* *********************** SetupPrint ************************************* */
void AnalysisGasGain::SetupPrint() {
  cout<<" "<<endl;
  cout<<ntuplename.c_str()<<endl;
  cout<<histrootname.c_str()<<endl;
  cout<<"flag_stat flag_print "<<flag_stat<<" "<<flag_print<<endl;
  cout<<""<<endl;
}

/* *********************** Find HV segment ***********************************/
Int_t  AnalysisGasGain::doHVsegment(Float_t yloc,Int_t stn,Int_t rng,Int_t layer)
{
  // Use local Y to find HV segment number.
  Int_t segment=0; //Define Segment # as 0 if not found
  const Int_t nsegm3=3;
  const Int_t nsegm5=5;

  if(stn==1 && (rng==1 || rng==4)) segment=layer; // ME11 or ME14
  if(stn==1 && rng==2) { //ME12
    for(Int_t i=0;i<nsegm3; i++)
    if((yloc >= me12YlocHVsgmLow[i]) && (yloc <= me12YlocHVsgmHigh[i]))
      segment=i+1;
  }
  if(stn==1 && rng==3) { //ME13
    for(Int_t i=0;i<nsegm3; i++)
    if((yloc >= me13YlocHVsgmLow[i]) && (yloc <= me13YlocHVsgmHigh[i]))
      segment=i+1;
  }
  if(stn==2 && rng==1) { //ME21
    for(Int_t i=0;i<nsegm3; i++)
    if((yloc >= me21YlocHVsgmLow[i]) && (yloc <= me21YlocHVsgmHigh[i]))
      segment=i+1;
  }
  if(stn==3 && rng==1) { //ME31
    for(Int_t i=0;i<nsegm3; i++)
    if((yloc >= me31YlocHVsgmLow[i]) && (yloc <= me31YlocHVsgmHigh[i]))
      segment=i+1;
  }
  if(stn==4 && rng==1) { //ME41
    for(Int_t i=0;i<nsegm3; i++)
    if((yloc >= me41YlocHVsgmLow[i]) && (yloc <= me41YlocHVsgmHigh[i]))
      segment=i+1;
  }
  if((stn==2 || stn==3 || stn==4) && rng==2) { //ME234/2 
    for(Int_t i=0;i<nsegm5; i++)
    if((yloc >= me234_2YlocHVsgmLow[i]) && (yloc <= me234_2YlocHVsgmHigh[i]))
      segment=i+1;
  }
  return segment;
}

/* ************************** Analyze ************************************* */
void AnalysisGasGain::Analyze(HistMan *histos) {
  CycleTree(histos);
  histos->ClearHistMaps();
  myoutfilefortree1->cd();
  outputtree1->Write();
  myoutfilefortree1->Close();
  for(int i = 0; i<32;i++){
  myoutfilefortree[i]->cd();
  outputtree[i]->Write();
  myoutfilefortree[i]->Close();
  }
	if(debug_bool) std::cout<<"output will be there  "<<std::endl;
}

/* *******************    GetME14RecHits ********************************** Not used anymore*/ 
void AnalysisGasGain::GetME14RecHits(HistMan* histos) {

 for(Int_t irechit=0;irechit<frecHits2D_nRecHits2D;irechit++){            
   Int_t endcap=frecHits2D_ID_endcap[irechit];
   Int_t station=frecHits2D_ID_station[irechit];
   Int_t ring=frecHits2D_ID_ring[irechit]; 
   Int_t chamber=frecHits2D_ID_chamber[irechit];
   Int_t layer=frecHits2D_ID_layer[irechit];
   Double_t sumq=frecHits2D_SumQ[irechit];

   Int_t key_layer=100000*endcap+10000*station+1000*ring+10*chamber+layer;
   Int_t key_chmb=10000*endcap+ 1000*station+ 100*ring+chamber;
    
   if(m_nRecHitlayer.find(key_layer)== m_nRecHitlayer.end())
     m_nRecHitlayer[key_layer]=0;
    m_nRecHitlayer[key_layer]= m_nRecHitlayer[key_layer]+1;
   if(m_nRecHitchamber.find(key_chmb)== m_nRecHitchamber.end())
     m_nRecHitchamber[key_chmb]=0;
    m_nRecHitchamber[key_chmb]= m_nRecHitchamber[key_chmb]+1;
   
   if(station==1 && ring==4 && sumq > 0.0) { //ME14
     if(m_nRecHitlayerME14.find(key_layer)== m_nRecHitlayerME14.end())
       m_nRecHitlayerME14[key_layer]=0;
      m_nRecHitlayerME14[key_layer]=m_nRecHitlayerME14[key_layer]+1;
   }
 } // end of for(Int_t irechit=0;irechit<frecHits2D_nRecHits2D;...)

  for(std::map<Int_t,Int_t>::iterator It=m_nRecHitlayer.begin(); It!= m_nRecHitlayer.end();++It) {
     Float_t nhits=(Float_t)(*It).second;
     histos->fill1DHist(nhits,"nhits_per_layer","","# of RecHit2D per layer","Entries",4,100,0.0,100.0,1.0,"Test");
  }
  for(std::map<Int_t,Int_t>::iterator It=m_nRecHitchamber.begin(); It!= m_nRecHitchamber.end();++It) {
     Float_t nhits=(Float_t)(*It).second;
     histos->fill1DHist(nhits,"nhits_per_chamber","","# of RecHit2D per chamber","Entries",4,1000,0.0,1000.0,1.0,"Test");
  }
 // now store the first hit from 3 hits of ME14 (SumQ is the same in all 3)
 // into final map m_RecHitlayerME14_final
 for(Int_t irechit=0;irechit<frecHits2D_nRecHits2D;irechit++) {
     Int_t endcap=frecHits2D_ID_endcap[irechit];
     Int_t station=frecHits2D_ID_station[irechit];
     Int_t ring=frecHits2D_ID_ring[irechit];
     Int_t chamber=frecHits2D_ID_chamber[irechit];
     Int_t layer=frecHits2D_ID_layer[irechit];
     Double_t xloc=frecHits2D_localX[irechit];
     Double_t yloc=frecHits2D_localY[irechit];
     Double_t sumq=frecHits2D_SumQ[irechit];

     if(station==1 && ring==4 && sumq > 0.0) { //ME14
      Int_t key_layer=100000*endcap+10000*station+1000*ring+10*chamber+layer;
      if(m_nRecHitlayerME14.find(key_layer)== m_nRecHitlayerME14.end())
        cout<<"Error, no m_nRecHitlayerME14 for key "<<key_layer<<endl;
      if(m_nRecHitlayerME14.find(key_layer)!= m_nRecHitlayerME14.end())   
        if(m_nRecHitlayerME14[key_layer]==3){// From what I understand this ensures there are only 3 hits (i.e. 3 times the same hit) in a given layer. (???).
          if(m_RecHitlayerME14_final.find(key_layer)== m_RecHitlayerME14_final.end()) {
	          m_RecHitlayerME14_final[key_layer]=zero6;
	          m_RecHitlayerME14_final[key_layer][0]=xloc;
            m_RecHitlayerME14_final[key_layer][1]=yloc;
            m_RecHitlayerME14_final[key_layer][2]=sumq;
            m_RecHitlayerME14_final[key_layer][3]=999.;
            m_RecHitlayerME14_final[key_layer][4]=999.;
            m_RecHitlayerME14_final[key_layer][5]=999.;
	        }
	       } // end of if(m_nRecHitlayerME14[key_layer]==3)
     }  // end of  if(station==1 && ring==4 && sumq > 0.0)
  }  // end of for(Int_t irechit=0;irechit<frecHits2D_nRecHits2D

} // end of GetME14RecHits

/* ******************** GetSegments (tracking segments)***********************/
void AnalysisGasGain::GetSegments(HistMan* histos) {
    histos->fill1DHist((Float_t)fcscSegments_recHitRecord_endcap->size(),"all_segments_per_event","","All segments per event","Entries",4,100,0.0,100.0,1.0,"Test");
    Int_t nhits_segm_event=0;
    
    for(UInt_t i=0;i<fcscSegments_recHitRecord_endcap->size();i++) {
     if(debug_bool) std::cout<<" for the event "<<_eventNb<<" segments number "<<fcscSegments_recHitRecord_endcap->size()<<std::endl;
     if(debug_bool) std::cout<<" number of hits "<<fcscSegments_recHitRecord_endcap->size()<<std::endl;
        if(i<100){ // limit 100 segments per event for key_segment
	        histos->fill1DHist((Float_t)(*fcscSegments_recHitRecord_endcap)[i].size(),"all_hits_per_segment","","All hits per segment","Entries",4,7,0.0,7.0,1.0,"Test");
          // use segments with hits in 4-6 layers only
           if((Int_t)(*fcscSegments_recHitRecord_endcap)[i].size() >= minhitpersegment ) {
           for(UInt_t j=0;j<(*fcscSegments_recHitRecord_endcap)[i].size();j++) {
       	     // here i-segment,j-hit
             Int_t endcap=(Int_t)(*fcscSegments_recHitRecord_endcap)[i][j];
             Int_t station=(Int_t)(*fcscSegments_recHitRecord_station)[i][j];
             Int_t ring=(Int_t)(*fcscSegments_recHitRecord_ring)[i][j];
             Int_t chamber=(Int_t)(*fcscSegments_recHitRecord_chamber)[i][j];
             Int_t layer=(Int_t)(*fcscSegments_recHitRecord_layer)[i][j];
             Double_t localX=(*fcscSegments_recHitRecord_localX)[i][j];
             Double_t localY=(*fcscSegments_recHitRecord_localY)[i][j];

             UInt_t key_segment=1000000*endcap+100000*station+10000*ring+100*chamber+i;
             std::cout<<" segments "<<fcscSegments_recHitRecord_endcap->size()<<" J : "<<j<<" local X "<<localX<< " local Y : "<<localY<<" chamber "<<(key_segment/100)<<std::endl;
             if(debug_bool)std::cout<<" key segment from segment "<<key_segment<<" layer "<<layer<<std::endl;
	           if(j==0) {
               nhits_segm_event=nhits_segm_event+(Int_t)(*fcscSegments_recHitRecord_endcap)[i].size();
               Int_t key_chamber=(Int_t)(key_segment/100);
               // count # of segments per chamber
               if(m_nsegments_chamber.find(key_chamber)==m_nsegments_chamber.end())
	              m_nsegments_chamber[key_chamber]=0;
	             m_nsegments_chamber[key_chamber]=m_nsegments_chamber[key_chamber]+1;
       	     } // end of if(j==0)
             
             if(m_cscSegments_recHitRecordX.find(key_segment)==m_cscSegments_recHitRecordX.end()) {
              m_cscSegments_recHitRecordX[key_segment]=zero6;
              m_cscSegments_recHitRecordY[key_segment]=zero6;
	           }
             m_cscSegments_recHitRecordX[key_segment][layer-1]=localX;
             m_cscSegments_recHitRecordY[key_segment][layer-1]=localY;
	         } // end of  for(UInt_t j=0;j<fcscSegments_recHitRecord_endcap[i]
         } // end of minhitpersegment =5
	      }   // end of if there is < 100 segments
    } // end of for(UInt_t i=0;i<fcscSegments_recHitRecord_endcap->size();i++)
    histos->fill1DHist((float)nhits_segm_event,"segment_nhits_per_event","","Segment hits per event","Entries",4,1000,0.0,1000.0,1.0,"Test");
    histos->fill1DHist((Float_t)m_nsegments_chamber.size(),"CSCs_with_segments_per_event","","# of CSCs with segments per event","Entries",4,100,0.0,100.0,1.0,"Test");

     for(std::map<Int_t,Int_t>::iterator It=m_nsegments_chamber.begin(); It!= m_nsegments_chamber.end();++It) {
       Float_t nsegm=(*It).second;
       histos->fill1DHist((Float_t)nsegm,"segments_per_chamber","","Number of segments per chamber","Entires",4,100,0.0,100.0,1.0,"Test");
     }

     std::map<int, int> n_times_chmb;
     // Here make map of single (per chamber) segments  
     // Earlier we use to make map of single(per chamber) segments, now we are collecting all possible segments
     // m_Single_cscSegments_recHitRecordX,*Y[key_chamber] from 
     // m_cscSegments_recHitRecordX,*Y using m_nsegments_chamber[key_chamber]=1
     // We need to just obtain information of first hit from the chamber, rest of the hits information is in segments ie. why it is [i][0]
     if(m_nsegments_chamber.size() > 0) {
     for(UInt_t i=0;i<fcscSegments_recHitRecord_endcap->size(); i++) {
       if((Int_t)(*fcscSegments_recHitRecord_endcap)[i].size() >= minhitpersegment){ 
        Int_t endcap=(Int_t)(*fcscSegments_recHitRecord_endcap)[i][0];
        Int_t station=(Int_t)(*fcscSegments_recHitRecord_station)[i][0];
        Int_t ring=(Int_t)(*fcscSegments_recHitRecord_ring)[i][0];
        Int_t chamber=(Int_t)(*fcscSegments_recHitRecord_chamber)[i][0];

        Int_t key_chmb=10000*endcap+1000*station+100*ring+chamber;
        
        if(m_nsegments_chamber.find(key_chmb)==m_nsegments_chamber.end()){
         if(debug_bool)  cout<<"Error, no segments for key_chmb="<<key_chmb<<" event "<<fEvent<<endl;
         n_times_chmb[key_chmb]= 0;
        }
        n_times_chmb[key_chmb]= n_times_chmb[key_chmb]+1;
        if(m_nsegments_chamber.find(key_chmb)!=m_nsegments_chamber.end()){
          // For now let's not just count single segment chambers but everything
          //std::cout<<m_nsegments_chamber[key_chmb]<<std::endl;
          if(m_nsegments_chamber[key_chmb]==1){
	        UInt_t key_segment=1000000*endcap+100000*station+10000*ring+100*chamber+i;  
           std::cout<<" here "<<std::endl;
	         if(m_Single_cscSegments_recHitRecordX.find(key_chmb) == m_Single_cscSegments_recHitRecordX.end()) { 
             if(m_cscSegments_recHitRecordX.find(key_segment)== m_cscSegments_recHitRecordX.end()) 
             cout<<"Error: no m_cscSegments_recHitRecordX with key_segment="<<key_segment<<endl;
             if(m_cscSegments_recHitRecordX.find(key_segment)!= m_cscSegments_recHitRecordX.end()){
		          m_Single_cscSegments_recHitRecordX[key_chmb]=m_cscSegments_recHitRecordX[key_segment];
		          m_Single_cscSegments_recHitRecordY[key_chmb]=m_cscSegments_recHitRecordY[key_segment];
              std::cout<<" considered one X : "<<m_Single_cscSegments_recHitRecordX[key_chmb][0]<<" Y "<<m_Single_cscSegments_recHitRecordY[key_chmb][0]<<" chamber "<<key_chmb<<" segment "<<key_segment<<std::endl;
              if(debug_bool)   std::cout<<" the single csc segment with value : key chamber : "<<key_chmb<<" segment "<<key_segment<<std::endl; 
	           }  // end of if(m_cscSegments_recHitRecordX.find(key_segment)
	         }   // end of if(m_Single_cscSegments_recHitRecordX.find(key_chmb)
	        }  // end of if(m_nsegments_chamber[key_chmb]==1)
      	}   // end of if(m_nsegments_chamber.find(key_chmb)!
         } // end of if((Int_t)(*fcscSegments_recHitRecord_endcap)[i].size() >=minhitpersegment )
      }  // end of for(UInt_t i=0;i<fcscSegments_recHitRecord_endcap->size()
     }  // end if(m_nsegments_chamber.size() > 0 
     
     // print number of times the chamber entry happened for key_chamber 
//     for(auto[key_chmb, value] : n_times_chmb){
//      std::cout<<" number of time "<<n_times_chmb[key_chmb]<<" chamber name "<<key_chmb<<std::endl;
//      std::cout<<" size of single segment  "<<m_Single_cscSegments_recHitRecordX[key_chmb].size()<<" chamber name "<<key_chmb<<std::endl;
//     }
     if(m_Single_cscSegments_recHitRecordX.size() > 0) {
      if(debug_bool) std::cout<<" single segment chambers size "<<m_Single_cscSegments_recHitRecordX.size()<<std::endl;
       histos->fill1DHist((Float_t)m_Single_cscSegments_recHitRecordX.size(),"segm_chambers_per_event","","segment chambers per event","Entries",4,100,0.0,100.0,1.0,"Test");

       Int_t nhits_single_segm_event=0;
       for(std::map<Int_t,std::vector <Double_t> >::iterator It=m_Single_cscSegments_recHitRecordX.begin(); It!= m_Single_cscSegments_recHitRecordX.end();++It) {
          Int_t key_chmb=(*It).first;
          for(Int_t i=0;i<6;i++) if(m_Single_cscSegments_recHitRecordX[key_chmb][i] > -999.0) nhits_single_segm_event++;
       }
       histos->fill1DHist((float)nhits_single_segm_event,"segm_rechits_per_event","","Segment hits per event","Entries",4,100,0.0,100.0,1.0,"Test");
     }
}

/* ***********************  GetTracks ********************************* */

void AnalysisGasGain::GetTracks(HistMan* histos) {
     ostringstream ss;
     // if(m_Single_cscSegments_recHitRecordX.size() > 0) {//There should be at least one segment in a chamber
     Int_t trackne=0;  //  e.g. it gives us the number of muons in an event which have CSC segments

     for(UInt_t i=0;i<fmuons_cscSegmentRecord_nRecHits->size();i++) //Loop over muons
     if(i<10 && (*fmuons_cscSegmentRecord_nRecHits)[i].size() > 0) 
     // i< 10 due to use in key for map
     trackne++;
     histos->fill1DHist((Float_t)trackne,"num_csc_muons_per_event","","Number of muons crossing CSCs per event","Entries",4,10,0.0,10.0,1.0,"Test");
     //// New commented off by Neha // if(trackne == 1) { // use events with single muon
     Int_t trackneindmx=0;
     int passed_sel = 0; 
     int hits_total = 0;
     // Loop over muons
     for(UInt_t i=0;i<fmuons_cscSegmentRecord_nRecHits->size();i++) {
        if(debug_new) std::cout<<" inside the loop for the muons "<<i<< " size "<<(*fmuons_cscSegmentRecord_nRecHits)[i].size()<<std::endl;
        // Going to all the segments in a muon
        if(i<10 && (*fmuons_cscSegmentRecord_nRecHits)[i].size() > 0  && 
        (fmuons_Zcand[i] && fmuons_isomuondzdxy[i] &&fmuons_TightId[i])) {
           //Last part to make sure that muon is isolated and passes Zmumu selection
          passed_sel++;
          // i< 10 due to use in key for map
          histos->fill1DHist((Float_t)(*fmuons_cscSegmentRecord_nRecHits)[i].size(),
          "segments_per_muon","","Number of CSC segments per muon passing Z selections","Entries",4,10,0.0,10.0,1.0,"Test");
          if((Int_t)i>trackneindmx) trackneindmx=(Int_t)i;

          Int_t segmindmx=0;

          // Going to all the hits in a segment
          Int_t total_muon_hits=0;
          if(debug_bool) std::cout<<" muon number "<<i<<" number of csc segments "<<(*fmuons_cscSegmentRecord_nRecHits)[i].size()<<std::endl;
          // before the j-loop for muon i
          for(UInt_t j=0;j<(*fmuons_cscSegmentRecord_nRecHits)[i].size();j++) {
            total_muon_hits = total_muon_hits + (*fmuons_cscSegmentRecord_nRecHits)[i][j];
            hits_total = hits_total + (*fmuons_cscSegmentRecord_nRecHits)[i][j];
            if(debug_bool) std::cout<<" hits "<<(*fmuons_cscSegmentRecord_nRecHits)[i][j]<<std::endl;
            if(j<10) { // limited due to use in key for map
              // process each chamber once per muon
              if((Int_t)j>segmindmx) segmindmx=(Int_t)j;
              Int_t endcap=(Int_t)(*fmuons_cscSegmentRecord_endcap)[i][j];
              Int_t station=(Int_t)(*fmuons_cscSegmentRecord_station)[i][j];
              Int_t ring=(Int_t)(*fmuons_cscSegmentRecord_ring)[i][j];
              Int_t chamber=(Int_t)(*fmuons_cscSegmentRecord_chamber)[i][j];
              Double_t localX=(*fmuons_cscSegmentRecord_localX)[i][j];
              Double_t localY=(*fmuons_cscSegmentRecord_localY)[i][j];
              Double_t nlayers=(*fmuons_cscSegmentRecord_nRecHits)[i][j];
              UInt_t key_segment=1000000*endcap+100000*station+10000*ring+100*chamber+i;
              UInt_t key_trksegm=1000000*endcap+100000*station+10000*ring+ 100*chamber+i;
              Int_t key_chmb=(Int_t)(key_trksegm/100);
              if(debug_bool) std::cout<<" segement number and key chamber "<<j<<" : "<<key_chmb<<std::endl;

              // to make sure there is a segment in the chamber
        	    if(m_nsegments_chamber.find(key_chmb) != m_nsegments_chamber.end())
              // The condition is to check number of segments in the chamber,and there should be just 1 segment in the chamber. This avoid complications arising from multiple segments in a chamber
              if(m_nsegments_chamber[key_chmb]==1){ 
              Int_t nhit=0;
              for(Int_t k=0;k<6;k++)  
		           if(m_Single_cscSegments_recHitRecordX[key_chmb][k]>-999.0) nhit++;
                
              if(nhit != (Int_t)nlayers) {
                if(debug_bool) cout<<"Warning - diff. # of hits "<<nhit<<" "<<nlayers<<" "<<key_trksegm<<"  "<<key_chmb<<"  "<<fEvent<<endl;
                histos->fill1DHist((Float_t)nlayers,"nhits_ne_nlayers","","# of layers given in track segment and different from original segment","Entires",4,8,0.0,8.0,1.0,"Test");
              }
              if(nhit==(Int_t)nlayers) {
                 Double_t sumx=0.0,sumy=0.0;
                 for(Int_t k=0;k<6;k++) {
                     Int_t key_layer=10*key_chmb+(k+1);
                     if(debug_bool) std::cout<<" layer record "<<key_layer<<std::endl;
                     // we will fill the single trk 
                     if(m_cscSegments_single_trk_recHitRecord.find(key_chmb)==m_cscSegments_single_trk_recHitRecord.end()) 
	          	         m_cscSegments_single_trk_recHitRecord[key_layer]=zero6;
                     if( m_Single_cscSegments_recHitRecordX.find(key_chmb)!= m_Single_cscSegments_recHitRecordX.end()){
                      if( m_Single_cscSegments_recHitRecordY.find(key_chmb)!= m_Single_cscSegments_recHitRecordY.end()){
                        if( m_Single_cscSegments_recHitRecordX[key_chmb][k] > -999.0 && m_Single_cscSegments_recHitRecordY[key_chmb][k] > -999.0 ){
                          if(debug_bool) std::cout<<"J value "<<j<<" position X : "<<m_Single_cscSegments_recHitRecordX[key_chmb][k]<<" position from muon "<<localX<<std::endl;
                          m_cscSegments_single_trk_recHitRecord[key_layer][0]=m_Single_cscSegments_recHitRecordX[key_chmb][k];
                          m_cscSegments_single_trk_recHitRecord[key_layer][1]=m_Single_cscSegments_recHitRecordY[key_chmb][k];
                        }
                      }
                     }
                     else{
                      m_cscSegments_single_trk_recHitRecord[key_layer][0]= -999.0;
                      m_cscSegments_single_trk_recHitRecord[key_layer][1]= -999.0;
                      if(debug_bool) std::cout<<" during checking the trk record position is zero"<<std::endl;
                     } 

                    m_cscSegments_single_trk_recHitRecord[key_layer][2]=-999.0;  
		                m_cscSegments_single_trk_recHitRecord[key_layer][3]=(Double_t) fmuons_pt[i];//Laurent: pt of the associated mu 
		                m_cscSegments_single_trk_recHitRecord[key_layer][4]=(Double_t) fmuons_eta[i];//Laurent: eta of the associated mu 
		                m_cscSegments_single_trk_recHitRecord[key_layer][5]=(Double_t) fmuons_phi[i];//Laurent: eta of the associated mu 
		      		      if(debug_bool) cout <<"pt, eta " <<fmuons_pt[i]<<", "<<fmuons_eta[i]<<endl;
                    sumx=sumx+localX;
                    sumy=sumy+localY;
                  }// end of layers k going through 6 layers
               } // end of checking (nhit==(Int_t)nlayers)
	          } // ended checking that there is a segment
	         } // ended of j <10 
         } // ended going through all the hits int the segment
          histos->fill1DHist(total_muon_hits,
          "number_of_hits_for_each_CSC_muon","","Number of hits per CSC muon in an event(passed Z selections)","Entries",4,100,0.0,100.0,1.0,"Test");
	     } // end of for(UInt_t j=0;j<(*fmuons_cscSegmentRecord_nRecHits)[i]... ended going through all the muons
      } // ended looking through the muons
       
       histos->fill1DHist((Float_t)passed_sel,"num_csc_muons_per_event_fromZ","","Number of CSC muons passing the Z selections (per event)","# of events",4,10,0.0,10.0,1.0,"Test");
       histos->fill1DHist(hits_total,
          "number_of_hits_per_event_for_CSC_muon","","Total number of hits per event for CSC muons(passed Z selections)","Entries",4,100,0.0,100.0,1.0,"Test");
        
   } // end of Get Tracks

/* *******************  GetRecHitsSumQ ********************************* */

void AnalysisGasGain::GetRecHitsSumQ(HistMan* histos) {
// To check if two rechits pass a key layer
    std::map<Int_t, int> rechit_count ; // number of rechits per key layer
    std::map<Int_t, std::map<Int_t, int>> rechit_counts ; // number of rechits per key layer
    std::map<Int_t, Double_t> temp_sumq; // sum of charges per key layer
    std::map<Int_t, Int_t> temp_nStrip; // sum of charges per key layer
    std::map<Int_t, Int_t> temp_nWire; // sum of charges per key layer
	   if(debug_bool) std::cout<<" to collect charges : event "<<_eventNb<<std::endl;
     if(m_cscSegments_single_trk_recHitRecord.size() > 0) {
       for(Int_t irechit=0;irechit<frecHits2D_nRecHits2D;irechit++) {
          Int_t ring=frecHits2D_ID_ring[irechit];
          // look at hits with SumQ>0 and not from ME1/4
      	  if((frecHits2D_SumQ[irechit] > 0.0) ) { // Commented out Laurent on 27/11/2017
            Int_t endcap=frecHits2D_ID_endcap[irechit];
            Int_t station=frecHits2D_ID_station[irechit];
            Int_t chamber=frecHits2D_ID_chamber[irechit];
     				Int_t nStrip=frecHits2D_nearestStrip[irechit];
     				Int_t nWire=frecHits2D_nearestWireGroup[irechit];
            Int_t layer=frecHits2D_ID_layer[irechit];
            Int_t key_layer=100000*endcap+10000*station+1000*ring+
                         10*chamber+layer;
            Double_t xloc=frecHits2D_localX[irechit];
            Double_t yloc=frecHits2D_localY[irechit];
            Double_t sumq=frecHits2D_SumQ[irechit];
            std::cout<<" x and y "<<xloc<<" : "<<yloc<<std::endl;
            if(m_cscSegments_single_trk_recHitRecord.find(key_layer)!=m_cscSegments_single_trk_recHitRecord.end()) {

              Double_t dx=xloc-m_cscSegments_single_trk_recHitRecord[key_layer][0];
              Double_t dy=yloc-m_cscSegments_single_trk_recHitRecord[key_layer][1];
	     
              Int_t hvsgm_number=doHVsegment(yloc,station,ring,layer);
              int iregion = GetRegionIdx(station,ring,hvsgm_number);
              rechit_counts[iregion][key_layer]++;
              rechit_count[key_layer]++;

              if(fabs(dx) < 0.0001 && fabs(dy) < 0.0001){
               std::cout<<" matching x and y "<<m_cscSegments_single_trk_recHitRecord[key_layer][0]<<" : "<<m_cscSegments_single_trk_recHitRecord[key_layer][1]<<" chamber "<<key_layer<<std::endl;
               if(debug_bool) std::cout<<"charge for key layer "<<key_layer<<" charge "<<sumq<<" event "<<_eventNb<<" chamber "<<chamber<<" nearest Strip "<<nStrip<<std::endl;
               if(debug_bool) std::cout<<"endcap "<<endcap<<" station "<<station<<" chamber "<<chamber<<" layer "<<layer<<" Event "<<fEvent<<" hit "<<irechit<<std::endl;
                temp_sumq[key_layer] = sumq; // sum of charges per key layer
								temp_nStrip[key_layer] = nStrip;
								temp_nWire[key_layer] = nWire;
              } // end of if(fabs(dx) < 0.0001 && fabs(dy) < 0.0001)
              else{
                if(debug_bool) cout<<"Warning, dx, dy not zero "<<dx<<" "<<dy<<" "<<key_layer<<"  "<<fEvent<<" key layer"<<key_layer<<endl;
              }

	    } // end of if(m_cscSegments_single_trk_recHitRecord.find...
	  } // end of  if((frecHits2D_SumQ[irechit] > 0.0)
  } // end of  for(Int_t irechit=0;irechit<frecHits2D_nRecHits2D;
// filling the number of hits in each layer of each hv segment
      for (const auto& entry : rechit_counts) {
            Int_t region = entry.first;
            std::map<Int_t, int> layer_count = entry.second;
            for(const auto & layer_entry : layer_count){
              TString chamber_type = GetRegionName(region);
              Int_t key_layer = layer_entry.first;
              int count = layer_entry.second;
              std::string hist_name = "number_hits_per_layer_"+std::string(chamber_type.Data());
              std::string hist_title = "Number of rechits per layer : "+ std::string(chamber_type.Data());
              histos->fill1DHist(count,hist_name,"",hist_title,"Entries",4,10,0.0,10.0,1.0,"Test");    //laurent: I commented this line:
            }
            // Only store sumq if there is exactly one rechit for this key layer
        }
      for (const auto& entry : rechit_count) {
        
            Int_t key_layer = entry.first;
            int count = entry.second;
            
            histos->fill1DHist(count,"number_hits_per_layer","","Number of rechits per layer","Entries",4,10,0.0,10.0,1.0,"Test");    //laurent: I commented this line:
            // Only store sumq if there is exactly one rechit for this key layer
            if (count == 1) {
              //    std::cout<<"only one rechit per key layer "<<key_layer<<"  "<<fEvent<<" sum "<<temp_sumq[key_layer]<<std::endl;
                m_cscSegments_single_trk_recHitRecord[key_layer][2] = temp_sumq[key_layer];
           } else {
                std::cout << "Multiple rechits for key layer: " << key_layer << ". sumQ not stored." <<std::endl;
           }
        }
        for(map<Int_t, std::vector <Double_t> >::iterator It=m_cscSegments_single_trk_recHitRecord.begin(); 
        It!=m_cscSegments_single_trk_recHitRecord.end(); It++) {
          if((*It).second[2] > 0.0) {
          Int_t key_layer=(*It).first;
     
          if(m_cscSegments_single_trk_recHitRecord_final.find(key_layer)==
      	    m_cscSegments_single_trk_recHitRecord_final.end())
            m_cscSegments_single_trk_recHitRecord_final[key_layer]=
	          m_cscSegments_single_trk_recHitRecord[key_layer];
					  // Adding the nearestStrip number in the final ntuple 
   					m_cscSegments_single_trk_recHitRecord_final[key_layer].push_back(static_cast<Double_t>(temp_nStrip[key_layer]));
					  m_cscSegments_single_trk_recHitRecord_final[key_layer].push_back(static_cast<Double_t>(temp_nWire[key_layer]));
          }
   	    } // end of  for(map<Int_t, std::vector <Double_t> >::iterator It=m_cscSegments_single_trk_recHitRecord.begin()

        Float_t df=(Float_t)m_cscSegments_single_trk_recHitRecord.size() - 
	           (Float_t)m_cscSegments_single_trk_recHitRecord_final.size();
	      
      	//Laurent        histos->fill1DHist(df,"diff_map_sizes","","Difference in two map sizes","Entries",4,10,0.0,10.0,1.0,"Test");
        // Printing switching off for now 
		    //if(df !=0.0) cout<<"Warning, different sizes "<<m_cscSegments_single_trk_recHitRecord.size()<<" "<<m_cscSegments_single_trk_recHitRecord_final.size()<<" "<<fEvent<<endl;
        histos->fill1DHist((Float_t)m_cscSegments_single_trk_recHitRecord_final.size(),"used_rechits_no_per_event","","Number of used rechits per event","Entries",4,100,0.0,100.0,1.0,"Test");    //laurent: I commented this line:

	      if(debug_bool) std::cout<<" finished collect charges "<<std::endl;
      } // end of if(m_cscSegments_single_trk_recHitRecord.size() > 0)
  }

/* ********************  AddME14RecHits ******************************* */

void AnalysisGasGain::AddME14RecHits() {
     if(m_RecHitlayerME14_final.size() > 0 )
       for(map<Int_t, std::vector <Double_t> >::iterator It=m_RecHitlayerME14_final.begin(); It!=m_RecHitlayerME14_final.end(); ++It) {
	 if(m_cscSegments_single_trk_recHitRecord_final.find((*It).first) !=
            m_cscSegments_single_trk_recHitRecord_final.end()) 
           cout<<"Error, key exists  "<<(*It).first<<endl;
         if(m_cscSegments_single_trk_recHitRecord_final.find((*It).first) ==
            m_cscSegments_single_trk_recHitRecord_final.end()) {

           m_cscSegments_single_trk_recHitRecord_final[(*It).first]=
	     (*It).second;
	 }

       } // end of for(map<Int_t, std::vector <Double_t> >::iterator It=m_RecHitlayerME14_final
}

/* *******************  FillSumQHists ********************************** */

void AnalysisGasGain::FillSumQHists(HistMan* histos) {

ostringstream ss;

       for(map<Int_t, std::vector <Double_t> >::iterator It=m_cscSegments_single_trk_recHitRecord_final.begin(); It!=m_cscSegments_single_trk_recHitRecord_final.end(); ++It) {

          Int_t key_layer=(*It).first;
          Int_t endcap=key_layer/100000;
          Int_t srcl=key_layer-100000*endcap; Int_t station=srcl/10000;
          Int_t rcl=srcl-10000*station;       Int_t ring=rcl/1000;
          Int_t cl=rcl-1000*ring; Int_t chamber=cl/10;
          Int_t layer=cl-10*chamber;

          Int_t key_layer_test=100000*endcap+10000*station+1000*ring+
                                                        10*chamber+layer;
          if(key_layer!=key_layer_test) cout<<"Error key_layer key_layer_test "<<key_layer<<" "<<key_layer_test<<endl;
          Int_t stationring=10*station+ring;

          // find HV segment
      	  Float_t yloc=(Float_t)(*It).second[1];
          Int_t hvsgm=doHVsegment(yloc,station,ring,layer);
	        //        histos->fill1DHist((float)hvsgm,"HV_segment","","HV segment #","Entries",4,6,0.0,6.0,1.0,"Hists"); //commented out by Laurent
          Float_t sumq=(Float_t)(*It).second[2];
     
	        _ptmuon =(Double_t)(*It).second[3];
	        _etamuon =(Double_t)(*It).second[4];
	        _phimuon =(Double_t)(*It).second[5];
		      _nearestStrip =  static_cast<Int_t>((*It).second[6]);
		      _nearestWireGroup =  static_cast<Int_t>((*It).second[7]);
          if(hvsgm > 0) { // skip rechits having no HV segment
            /* Following commented out by Laurent
            ss.str("");
            ss<<"SumQ_";
            ss<<endcap<<station<<ring;
            if(chamber<10) ss<<0;
            ss<<chamber<<layer;
            if(stationring != 11 && stationring!=14) ss<<hvsgm;

    	      histos->fill1DHist(sumq,ss.str().c_str(),ss.str().c_str(),"recHit SumQ","Entries",4,4000,0.0,4000.0,1.0,"HistsSegm"); 
	          //	    here to add q vs lumi
       	    //best structure is to have a tree with sumq, lumi, pressure, ..

            ss.str("");
            ss<<"SumQ_";
            ss<<station<<ring;
            histos->fill1DHist(sumq,ss.str().c_str(),ss.str().c_str(),"recHit SumQ","Entries",4,4000,0.0,4000.0,1.0,"Hists");

            histos->fill1DHist(sumq,"SumQ","","recHit SumQ","Entries",4,4000,0.0,4000.0,1.0,"Hists");

            if(stationring == 11 || stationring==14) {
              ss.str(""); ss<<"SumQ_ME11_ME14";
              histos->fill1DHist(sumq,ss.str().c_str(),ss.str().c_str(),"recHit SumQ","Entries",4,4000,0.0,4000.0,1.0,"Hists");
              ss.str(""); ss<<"Yloc_ME11_ME14";
               histos->fill1DHist(yloc,ss.str().c_str(),ss.str().c_str(),"recHitYloc, cm","Entries",4,200,-100.0,100.0,1.0,"Hists");
	    } // end of if(stationring == 11 || stationring==14)
	    */

	    _rhid = 1000000 * endcap + 100000*station + 10000*ring + 100*chamber + 10*layer  + hvsgm ;
	    _stationring = station * 10+ ring;
	    _runNb = fRun ;
	    _lumiBlock =fLumiSect ;
	    _bunchcrossing = fBunchCrossing;
	    _eventNb = fEvent;
	    _timesecond =ftimeSecond ;

	    /*	    cout <<"vt "<< (int)fvertex_nVertex <<
	      ", "<<  (unsigned int)fvertex_nVertex<<
	      ", "<<  (float)fvertex_nVertex<<
	      ", "<<  (double)fvertex_nVertex<< 
	      ", "<<  (unsigned long)fvertex_nVertex<<
	      ", "<<  (long)fvertex_nVertex<<
	      endl;// ", "<<_n_PV <<endl;*/
//	    _n_PV = fvertex_nVertex;

	    _rhsumQ_RAW = sumq;
	    std::pair<double,double>  gasgainandhv = UncorrGasGain_HVInitial(sumq,fRun,_stationring,_rhid);
	    //	    cout << "run, sumq, stationring, rhid: " << fRun <<", "<<sumq<<", "<<_stationring<<", "<<_rhid <<endl;
	    _rhsumQ =  gasgainandhv.first;
	    _HV =  gasgainandhv.second;
	    _current = 0;

        histos->fill1DHist((Float_t)_rhsumQ_RAW,"charge_for_selected_hits","","ADC charge for final used rechits","Entries",4,100,0.0,3000.0,1.0,"Test");
	    int iregion = GetRegionIdx(station,ring,hvsgm);
			if(debug_bool_region)std::cout<<" value of the region in each rechit "<<iregion<<" charge "<<_rhsumQ<<" event Nb"<<_eventNb<<" hv segment "<<hvsgm<<std::endl;
	    outputtree1->Fill();
	    if(iregion >=0)  outputtree[iregion]->Fill();
	    else cout <<"region not found! " <<endl;
	    
	   if(debug_bool) std::cout<<" found the region and filled variables : run : event "<<_runNb<<" "<<_eventNb<<std::endl;

	  } // end if (hvsgm > 0)
       } // end of   for(map<Int_t, std::vector <Double_t> >::iterator It=m_cscSegments_single_trk_recHitRecord_final.begin()

}

/* ********************  CycleTree ************************************* */

void AnalysisGasGain::CycleTree(HistMan* histos) {

  m_RunEvent.clear();
  m_Run_usedevents.clear();
  m_Run_usedhits.clear();
  m_RunStart.clear();
  m_RunEnd.clear();

  TFile *histrootfile = new TFile(histrootname.c_str(), "RECREATE");
  TFile *f =  TFile::Open(ntuplename.c_str());
  if(debug_bool) {std::cout<<" name of the chamber in start "<<histrootname.c_str()<<std::endl;
   std::cout<<" opened the root file and making tree "<<std::endl;
	}
  TTree *tree = (TTree*)f->Get("cscRootMaker/Events");
  nentries = tree->GetEntries();
  cout<<"Total entries       "<<nentries<<endl;
  if(flag_stat>0) nentries=flag_stat;
  cout<<"Entries to read in  "<<nentries<<endl;
  cout<<endl;

	if(debug_bool) std::cout<<"branches to be declared"<<std::endl;
  // Get branches
  TBranch *b_Run=tree->GetBranch("Run");
	if(debug_program) std::cout<<"first branch declared"<<std::endl;
  TBranch *b_LumiSect=tree->GetBranch("LumiSect");
//  TBranch *b_vertex_nVertex=tree->GetBranch("vertex_nVertex");
  TBranch *b_Event=tree->GetBranch("Event");
  TBranch *b_timeSecond=tree->GetBranch("timeSecond");
  TBranch *b_BunchCrossing=tree->GetBranch("BunchCrossing");

  TBranch *b_recHits2D_nRecHits2D=tree->GetBranch("recHits2D_nRecHits2D");
  TBranch *b_recHits2D_ID_endcap=tree->GetBranch("recHits2D_ID_endcap");
  TBranch *b_recHits2D_ID_station=tree->GetBranch("recHits2D_ID_station");
  TBranch *b_recHits2D_ID_ring=tree->GetBranch("recHits2D_ID_ring");
  TBranch *b_recHits2D_ID_chamber=tree->GetBranch("recHits2D_ID_chamber");
  TBranch *b_recHits2D_ID_layer=tree->GetBranch("recHits2D_ID_layer");
  TBranch *b_recHits2D_nearestStrip=tree->GetBranch("recHits2D_nearestStrip");
  TBranch *b_recHits2D_nearestWireGroup=tree->GetBranch("recHits2D_nearestWireGroup");
  TBranch *b_recHits2D_localX=tree->GetBranch("recHits2D_localX");
  TBranch *b_recHits2D_localY=tree->GetBranch("recHits2D_localY");
  TBranch *b_recHits2D_SumQ=tree->GetBranch("recHits2D_SumQ");
  
  TBranch *b_cscSegments_recHitRecord_endcap=tree->GetBranch("cscSegments_recHitRecord_endcap");
  TBranch *b_cscSegments_recHitRecord_station=tree->GetBranch("cscSegments_recHitRecord_station");
  TBranch *b_cscSegments_recHitRecord_ring=tree->GetBranch("cscSegments_recHitRecord_ring");
  TBranch *b_cscSegments_recHitRecord_chamber=tree->GetBranch("cscSegments_recHitRecord_chamber");
  TBranch *b_cscSegments_recHitRecord_layer=tree->GetBranch("cscSegments_recHitRecord_layer");
  TBranch *b_cscSegments_recHitRecord_localX=tree->GetBranch("cscSegments_recHitRecord_localX");
  TBranch *b_cscSegments_recHitRecord_localY=tree->GetBranch("cscSegments_recHitRecord_localY");

  TBranch *b_muons_cscSegmentRecord_nRecHits=tree->GetBranch("muons_cscSegmentRecord_nRecHits");
  TBranch *b_muons_cscSegmentRecord_endcap=tree->GetBranch("muons_cscSegmentRecord_endcap");
  TBranch *b_muons_cscSegmentRecord_station=tree->GetBranch("muons_cscSegmentRecord_station");
  TBranch *b_muons_cscSegmentRecord_ring=tree->GetBranch("muons_cscSegmentRecord_ring");
  TBranch *b_muons_cscSegmentRecord_chamber=tree->GetBranch("muons_cscSegmentRecord_chamber");
  TBranch *b_muons_cscSegmentRecord_localX=tree->GetBranch("muons_cscSegmentRecord_localX");
  TBranch *b_muons_cscSegmentRecord_localY=tree->GetBranch("muons_cscSegmentRecord_localY");

  TBranch *b_muons_nMuons=tree->GetBranch("muons_nMuons");

	TBranch *b_nb_trigger_matched=tree->GetBranch("nb_trigger_matched");
	TBranch *b_pT_matched=tree->GetBranch("pT_matched");
  TBranch *b_eta_matched=tree->GetBranch("eta_matched");
  TBranch *b_phi_matched=tree->GetBranch("phi_matched");
  TBranch *b_Id_matched=tree->GetBranch("Id_matched");

  TBranch *b_muons_charge=tree->GetBranch("muons_charge");
  TBranch *b_muons_pt=tree->GetBranch("muons_pt");
  TBranch *b_muons_eta=tree->GetBranch("muons_eta");
  TBranch *b_muons_phi=tree->GetBranch("muons_phi");
  TBranch *b_muons_dz=tree->GetBranch("muons_dz");
  TBranch *b_muons_dxy=tree->GetBranch("muons_dxy");
  TBranch *b_muons_isoCH03=tree->GetBranch("muons_isoCH03");
  TBranch *b_muons_isoNH03=tree->GetBranch("muons_isoNH03");
  TBranch *b_muons_isoPhot03=tree->GetBranch("muons_isoPhot03");
  TBranch *b_muons_isoPU03=tree->GetBranch("muons_isoPU03");

  TBranch *b_muons_isoCH04=tree->GetBranch("muons_isoCH04");
  TBranch *b_muons_isoNH04=tree->GetBranch("muons_isoNH04");
  TBranch *b_muons_isoPhot04=tree->GetBranch("muons_isoPhot04");
  TBranch *b_muons_isoPU04=tree->GetBranch("muons_isoPU04");


 TBranch *b_muons_isTrackerMuon = tree->GetBranch("muons_isTrackerMuon");
 TBranch *b_muons_isGlobalMuon = tree->GetBranch("muons_isGlobalMuon");
 TBranch *b_muons_isPFMuon = tree->GetBranch("muons_isPFMuon");
 TBranch *b_muons_globalTrackNormalizedChi2 = tree->GetBranch("muons_globalTrackNormalizedChi2");
 TBranch *b_muons_globalTrackNumberOfValidMuonHits= tree->GetBranch("muons_globalTrackNumberOfValidMuonHits");
 TBranch *b_muons_trackNumberOfValidHits = tree->GetBranch("muons_trackNumberOfValidHits");
 TBranch *b_muons_trackerLayers = tree->GetBranch("muons_trackerLayers");
 TBranch *b_muons_trackerValidPixelHits = tree->GetBranch("muons_trackerValidPixelHits");
 TBranch *b_muons_numberOfMatches = tree->GetBranch("muons_numberOfMatches");
 TBranch *b_muons_numberOfMatchedStations = tree->GetBranch("muons_numberOfMatchedStations");
 TBranch *b_muons_numberOfChambers = tree->GetBranch("muons_numberOfChambers");
 TBranch *b_muons_numberOfSegments = tree->GetBranch("muons_numberOfSegments");
	if(debug_bool) std::cout<<"all branch declared"<<std::endl;
  // Set addresses
  b_Run->SetAddress(&fRun);
  b_Event->SetAddress(&fEvent);
  b_LumiSect->SetAddress(&fLumiSect);
  b_timeSecond->SetAddress(&ftimeSecond);
//  b_vertex_nVertex->SetAddress(&fvertex_nVertex);
  
	if(debug_bool) std::cout<<"did setup basic branches :1 "<<std::endl;
  b_recHits2D_nRecHits2D->SetAddress(&frecHits2D_nRecHits2D);
  b_recHits2D_ID_endcap->SetAddress(frecHits2D_ID_endcap); // no & for array
  b_recHits2D_ID_station->SetAddress(frecHits2D_ID_station);
  b_recHits2D_ID_ring->SetAddress(frecHits2D_ID_ring);
  b_recHits2D_ID_chamber->SetAddress(frecHits2D_ID_chamber);
  b_recHits2D_ID_layer->SetAddress(frecHits2D_ID_layer);
  b_recHits2D_nearestStrip->SetAddress(frecHits2D_nearestStrip);
  b_recHits2D_nearestWireGroup->SetAddress(frecHits2D_nearestWireGroup);
  b_recHits2D_localX->SetAddress(frecHits2D_localX);
  b_recHits2D_localY->SetAddress(frecHits2D_localY);
  b_recHits2D_SumQ->SetAddress(frecHits2D_SumQ); 

	if(debug_bool) std::cout<<"did setup basic branches :2 "<<std::endl;
//  b_cscSegments_recHitRecord_endcap->SetAddress(fcscSegments_recHitRecord_endcap);
//  b_cscSegments_recHitRecord_station->SetAddress(fcscSegments_recHitRecord_station);
//  b_cscSegments_recHitRecord_ring->SetAddress(fcscSegments_recHitRecord_ring);
//  b_cscSegments_recHitRecord_chamber->SetAddress(fcscSegments_recHitRecord_chamber);
//  b_cscSegments_recHitRecord_layer->SetAddress(fcscSegments_recHitRecord_layer);
//  b_cscSegments_recHitRecord_localX->SetAddress(fcscSegments_recHitRecord_localX);
//  b_cscSegments_recHitRecord_localY->SetAddress(fcscSegments_recHitRecord_localY);
//
//  b_muons_cscSegmentRecord_nRecHits->SetAddress(fmuons_cscSegmentRecord_nRecHits);
//  b_muons_cscSegmentRecord_endcap->SetAddress(fmuons_cscSegmentRecord_endcap);
//  b_muons_cscSegmentRecord_station->SetAddress(fmuons_cscSegmentRecord_station);
//  b_muons_cscSegmentRecord_ring->SetAddress(fmuons_cscSegmentRecord_ring);
//  b_muons_cscSegmentRecord_chamber->SetAddress(fmuons_cscSegmentRecord_chamber);
//  b_muons_cscSegmentRecord_localX->SetAddress(fmuons_cscSegmentRecord_localX);
//  b_muons_cscSegmentRecord_localY->SetAddress(fmuons_cscSegmentRecord_localY);

  b_cscSegments_recHitRecord_endcap->SetAddress(&fcscSegments_recHitRecord_endcap);
  b_cscSegments_recHitRecord_station->SetAddress(&fcscSegments_recHitRecord_station);
  b_cscSegments_recHitRecord_ring->SetAddress(&fcscSegments_recHitRecord_ring);
  b_cscSegments_recHitRecord_chamber->SetAddress(&fcscSegments_recHitRecord_chamber);
  b_cscSegments_recHitRecord_layer->SetAddress(&fcscSegments_recHitRecord_layer);
  b_cscSegments_recHitRecord_localX->SetAddress(&fcscSegments_recHitRecord_localX);
  b_cscSegments_recHitRecord_localY->SetAddress(&fcscSegments_recHitRecord_localY);

	if(debug_bool) std::cout<<"did setup basic branches :3 "<<std::endl;
  b_muons_cscSegmentRecord_nRecHits->SetAddress(&fmuons_cscSegmentRecord_nRecHits);
  b_muons_cscSegmentRecord_endcap->SetAddress(&fmuons_cscSegmentRecord_endcap);
  b_muons_cscSegmentRecord_station->SetAddress(&fmuons_cscSegmentRecord_station);
  b_muons_cscSegmentRecord_ring->SetAddress(&fmuons_cscSegmentRecord_ring);
  b_muons_cscSegmentRecord_chamber->SetAddress(&fmuons_cscSegmentRecord_chamber);
  b_muons_cscSegmentRecord_localX->SetAddress(&fmuons_cscSegmentRecord_localX);
  b_muons_cscSegmentRecord_localY->SetAddress(&fmuons_cscSegmentRecord_localY);

	if(debug_bool) std::cout<<"did setup basic branches :4 "<<std::endl;
  b_muons_nMuons->SetAddress(&fmuons_nMuons);
 
	if(debug_bool) std::cout<<"did setup basic branches :4 second "<<std::endl;
 	b_nb_trigger_matched->SetAddress(&nb_trigger_matched);
	if(debug_bool) std::cout<<"did setup basic branches :5 "<<std::endl;
 	b_pT_matched->SetAddress(&pT_matched);
  b_eta_matched->SetAddress(&eta_matched);
  b_phi_matched->SetAddress(&phi_matched);
  b_Id_matched->SetAddress(&Id_matched);
	if(debug_bool) std::cout<<"did setup basic branches :6 "<<std::endl;

	b_muons_charge->SetAddress(fmuons_charge);
  b_muons_pt->SetAddress(fmuons_pt);
  b_muons_eta->SetAddress(fmuons_eta);
  b_muons_phi->SetAddress(fmuons_phi);
  b_muons_dz->SetAddress(fmuons_dz);
  b_muons_dxy->SetAddress(fmuons_dxy);
  b_muons_isoCH03->SetAddress(fmuons_isoCH03);
  b_muons_isoNH03->SetAddress(fmuons_isoNH03);
  b_muons_isoPU03->SetAddress(fmuons_isoPU03);
  b_muons_isoPhot03->SetAddress(fmuons_isoPhot03);

	if(debug_bool) std::cout<<"did setup basic branches :7 "<<std::endl;
  b_muons_isoCH04->SetAddress(fmuons_isoCH04);
  b_muons_isoNH04->SetAddress(fmuons_isoNH04);
  b_muons_isoPU04->SetAddress(fmuons_isoPU04);
  b_muons_isoPhot04->SetAddress(fmuons_isoPhot04);

  b_muons_isTrackerMuon->SetAddress(fmuons_isTrackerMuon);
  b_muons_isGlobalMuon->SetAddress(fmuons_isGlobalMuon);
  b_muons_isPFMuon->SetAddress(fmuons_isPFMuon);
  //tree->SetBranchAddress("muons_globalTrackNormalizedChi2", fmuons_globalTrackNormalizedChi2);

  b_muons_globalTrackNormalizedChi2->SetAddress(fmuons_globalTrackNormalizedChi2);
  b_muons_globalTrackNumberOfValidMuonHits->SetAddress(fmuons_globalTrackNumberOfValidMuonHits);
  b_muons_trackNumberOfValidHits->SetAddress(fmuons_trackNumberOfValidHits);
  b_muons_numberOfMatchedStations->SetAddress(fmuons_numberOfMatchedStations);
  b_muons_trackerLayers->SetAddress(fmuons_trackerLayers);
  b_muons_trackerValidPixelHits->SetAddress(fmuons_trackerValidPixelHits);
  b_muons_numberOfMatches->SetAddress(fmuons_numberOfMatches);
  b_muons_numberOfChambers->SetAddress(fmuons_numberOfChambers);
  b_muons_numberOfSegments->SetAddress(fmuons_numberOfSegments);

	if(debug_bool) std::cout<<"setted addresses for all the  branch "<<std::endl;
fcscSegments_recHitRecord_endcap  = 0; 
fcscSegments_recHitRecord_station = 0; 
fcscSegments_recHitRecord_ring    = 0;  
fcscSegments_recHitRecord_chamber = 0; 
fcscSegments_recHitRecord_layer   = 0; 
fcscSegments_recHitRecord_localX  = 0; 
fcscSegments_recHitRecord_localY  = 0;

fmuons_cscSegmentRecord_nRecHits  = 0;
fmuons_cscSegmentRecord_endcap    = 0;
fmuons_cscSegmentRecord_station   = 0;
fmuons_cscSegmentRecord_ring      = 0;
fmuons_cscSegmentRecord_chamber   = 0;
fmuons_cscSegmentRecord_localY    = 0;
fmuons_cscSegmentRecord_localX    = 0;


  // *********************************************
  // ***  Cycling over tree
  // *********************************************


  ULong64_t runnb_previous_event(0),  lumis_previous_event(0);

  //if(debug_program)  std::cout<<"entering entry loop "<<std::endl;
    std::cout<<"entering entry loop "<<std::endl;
  int passed_Zevents = 0;
  int passed_Zevents_one = 0;
  int passed_Zevents_two = 0;
  int not_passed_Zevents = 0;
  int not_passed_Zevents_zero = 0;
  int not_passed_Zevents_multiple = 0;
  int not_matched_events =0;
  int valid_muon_indices = 0;
  int non_valid_muon_indices = 0;
  int matched_leading_trigger_events = 0;
  int matched_subleading_trigger_events = 0;
  int passed_Zevents_full_range = 0;
  int not_passed_Zevents_full_range = 0;

  int Nevents_trigger_matched = 0;
  int Nevents_not_trigger_matched = 0;
  int Nevents_not_trigger_matched_pTcut = 0;
  int Nevents_with_two_isolated_muons = 0;
  int Nevents_with_two_tight_isolated_muons = 0;

  double pT_cut = 28;

for(Int_t ient=0;ient<nentries;ient++) {
//  for(Int_t ient=0;ient<5;ient++) {
   
  //if(debug_program)  std::cout<<"time to load a entry"<<std::endl;
     if(debug_new) std::cout<<"time to load a entry"<<ient<<std::endl;
     b_Run->GetEntry(ient);
     b_Event->GetEntry(ient);
     b_LumiSect->GetEntry(ient);
     b_timeSecond->GetEntry(ient);
     
//     b_vertex_nVertex->GetEntry(ient);
     b_BunchCrossing->GetEntry(ient);

     b_recHits2D_nRecHits2D->GetEntry(ient);
     b_recHits2D_ID_endcap->GetEntry(ient);
     b_recHits2D_ID_station->GetEntry(ient);
     b_recHits2D_ID_ring->GetEntry(ient);
     b_recHits2D_ID_chamber->GetEntry(ient);
     b_recHits2D_nearestStrip->GetEntry(ient);
     b_recHits2D_nearestWireGroup->GetEntry(ient);
     b_recHits2D_ID_layer->GetEntry(ient);
     b_recHits2D_localX->GetEntry(ient);
     b_recHits2D_localY->GetEntry(ient);
     b_recHits2D_SumQ->GetEntry(ient);
     
		 b_nb_trigger_matched->GetEntry(ient);
		 b_pT_matched->GetEntry(ient);
		 b_eta_matched->GetEntry(ient);
		 b_phi_matched->GetEntry(ient);
		 b_Id_matched->GetEntry(ient);
     
  if(debug_program)  std::cout<<"loaded few more of the branches"<<std::endl;
     b_cscSegments_recHitRecord_endcap->GetEntry(ient);
     b_cscSegments_recHitRecord_station->GetEntry(ient);
     b_cscSegments_recHitRecord_ring->GetEntry(ient);
     b_cscSegments_recHitRecord_chamber->GetEntry(ient);
     b_cscSegments_recHitRecord_layer->GetEntry(ient);
     b_cscSegments_recHitRecord_localX->GetEntry(ient);
     b_cscSegments_recHitRecord_localY->GetEntry(ient);

     b_muons_cscSegmentRecord_nRecHits->GetEntry(ient);
     b_muons_cscSegmentRecord_endcap->GetEntry(ient);
     b_muons_cscSegmentRecord_station->GetEntry(ient);
     b_muons_cscSegmentRecord_ring->GetEntry(ient);
     b_muons_cscSegmentRecord_chamber->GetEntry(ient);
     b_muons_cscSegmentRecord_localX->GetEntry(ient);
     b_muons_cscSegmentRecord_localY->GetEntry(ient);

     b_muons_pt->GetEntry(ient);
     b_muons_charge->GetEntry(ient);
     b_muons_eta->GetEntry(ient);
     b_muons_phi->GetEntry(ient);
     b_muons_dz->GetEntry(ient);
     b_muons_dxy->GetEntry(ient);
     b_muons_isoCH03->GetEntry(ient);
     b_muons_isoNH03->GetEntry(ient);
     b_muons_isoPhot03->GetEntry(ient);
     b_muons_isoPU03->GetEntry(ient);
     b_muons_isoCH04->GetEntry(ient);
     b_muons_isoNH04->GetEntry(ient);
     b_muons_isoPhot04->GetEntry(ient);
     b_muons_isoPU04->GetEntry(ient);

     b_muons_globalTrackNormalizedChi2->GetEntry(ient);
     b_muons_globalTrackNumberOfValidMuonHits->GetEntry(ient);
     b_muons_trackNumberOfValidHits->GetEntry(ient);
     b_muons_numberOfMatchedStations->GetEntry(ient);
     b_muons_trackerLayers->GetEntry(ient);
     b_muons_trackerValidPixelHits->GetEntry(ient);
     b_muons_numberOfMatches->GetEntry(ient);
     b_muons_numberOfChambers->GetEntry(ient);
     b_muons_numberOfSegments->GetEntry(ient);
     m_nRecHitlayer.clear();
     m_nRecHitchamber.clear();
     m_nRecHitlayerME14.clear();
     m_RecHitlayerME14_final.clear();
     m_cscSegments_recHitRecordX.clear();
     m_cscSegments_recHitRecordY.clear();
     m_nsegments_chamber.clear();
     m_muon_segm.clear();
     m_Single_cscSegments_recHitRecordX.clear();
     m_Single_cscSegments_recHitRecordY.clear();
     m_cscSegments_single_trk_recHitRecord.clear();
     m_cscSegments_single_trk_recHitRecord_final.clear();

     double pi = TMath::Pi();
     if(year=="2016")  _pressure = getpressure2016(ftimeSecond);
		 else if(year=="2017")  _pressure = getpressure2017(ftimeSecond);
		 else if(year=="2018")  _pressure = getpressure2018(ftimeSecond);
		 _temperature =0;
     double mass_mu = 0.1056; 
     if(debug_bool)  std::cout<<"loading of branches done`"<<std::endl;
     if(debug_new) std::cout<<"year "<<year<<std::endl;


    // Variables to count the events passing each selection step
    double isolation_value, isolation_value_04; 
    double isolation_value_one, isolation_value_two;
     for(int iM = 0; iM< fmuons_nMuons;iM++) {
			 fmuons_Zcand[iM] = false; fmuons_isomuondzdxy[iM] = false;
			 fmuons_TightId[iM] = false;
       fmuons_matched_trigger[iM] = false;
      }
     //To Skim: 2 muons pt 10, 70<M(mumu)<110
     passZmumusel= false;
     passisomuondzdxy = false;
     double mass =-1;
		// Variables to find the one Z candidate
		int first_index = 999, second_index = 999;
		int prev_index_iM, prev_index_jM ; 
		double nominal_mass = 91.1876; 
	  //double prev_mass;
    int nZcandidates = 0;
    int nZcandidates_narrow = 0;
    double best_mass_diff = 999.0;

     histos->fill1DHist(fmuons_nMuons,"number_muons_per_event","","Total number of muons per event","Entries",4,10,0.0,10.0,1.0,"Test");   
     if(debug_new) std::cout<<" number of muons in the event "<<ient<<" : "<<fmuons_nMuons<<std::endl;
     // Checking pT, eta, phi , isolation value for each muon
     for(int tM=0; tM< fmuons_nMuons;tM++){
            isolation_value = (fmuons_isoCH03[tM] + std::max (0.0, fmuons_isoNH03[tM] + fmuons_isoPhot03[tM] - 0.5*(fmuons_isoPU03[tM]) ) )/ fmuons_pt[tM]; 
            isolation_value_04 = (fmuons_isoCH04[tM] + std::max (0.0, fmuons_isoNH04[tM] + fmuons_isoPhot04[tM] - 0.5*(fmuons_isoPU04[tM]) ) )/ fmuons_pt[tM]; 
            histos->fill1DHist(fmuons_pt[tM],"pT","","muon pT before selections (GeV)","Entries",4,40,0,200.0,1.0,"Test");
            histos->fill1DHist(fmuons_eta[tM],"eta","","muon #eta","Entries",4,100,-2.5,2.5,1.0,"Test");
            histos->fill1DHist(fmuons_phi[tM],"phi","","muon #phi","Entries",4,100,-pi,pi,1.0,"Test");
            histos->fill1DHist(abs(fmuons_dz[tM]),"dz_PV","","muon track distance from PV along z axis |dz(PV)|","Entries",4,50,0.0,1.0,1.0,"Test");
            histos->fill1DHist(abs(fmuons_dxy[tM]),"dxy_PV","","muon track distance from PV along XY plane |dxy(PV)|","Entries",4,50,0.0,1.0,1.0,"Test");
            histos->fill1DHist(fmuons_charge[tM],"muon_charge","","Muon charge ","Entries",4, 26,-13,13,1.0,"Test");
           // Muon isolation plots
            histos->fill1DHist(isolation_value,"muon_isolation","","relative isolation I(PF) ","Entries",4,400,0,20,1.0,"Test");
            histos->fill1DHist(isolation_value_04,"muon_isolation_04","","relative isolation I(PF) : radius (0.4) ","Entries",4,400,0,0.2,1.0,"Test");
            // This were some debug plots for the isolation values, commented out by Neha
///            if(isolation_value==0){
///            histos->fill1DHist(isolation_value,"muon_isolation_overall_zero","","muon isolation overall only zero entries ","Entries",4,400,0,0.2,1.0,"Test");
///            histos->fill1DHist(fmuons_isoCH03[tM],"muon_isolation_overall_zero_isoCH03","","muon isolation overall only zero entries : isoCH03 ","Entries",4,400,0,0.2,1.0,"Test");
///            histos->fill1DHist(fmuons_isoNH03[tM],"muon_isolation_overall_zero_isoNH03","","muon isolation overall only zero entries : isoNH03 ","Entries",4,400,0,0.2,1.0,"Test");
///            histos->fill1DHist(fmuons_isoPhot03[tM],"muon_isolation_overall_zero_isoPhot03","","muon isolation overall only zero entries : isoPhot03 ","Entries",4,400,0,0.2,1.0,"Test");
///            histos->fill1DHist(fmuons_isoPU03[tM],"muon_isolation_overall_zero_isoPU03","","muon isolation overall only zero entries : isoPU03 ","Entries",4,400,0,0.2,1.0,"Test");
///            }
///            if(fmuons_isoCH03[tM]==0 && fmuons_isoNH03[tM]==0 && fmuons_isoPhot03[tM]==0 && fmuons_isoPU03[tM]==0){
///            histos->fill1DHist(isolation_value,"muon_isolation_individual_zero","","muon isolation overall : when everything is zero ","Entries",4,400,0,0.2,1.0,"Test");
///            }
///            if(fmuons_isoCH03[tM]==0 && fmuons_isoNH03[tM]==0 && fmuons_isoPhot03[tM]==0){
///            histos->fill1DHist(isolation_value_second,"muon_isolation_second_individual_zero","","muon isolation : when everything is zero ","Entries",4,400,0,0.2,1.0,"Test");
///            }
///            if(isolation_value!=0) {
///            histos->fill1DHist(isolation_value,"muon_isolation_overall_without_zerovalue","","relative isolation I(PF) ","Entries",4,400,0,2,1.0,"Test");
///            } 
      } // end of muon loop
    if(debug_new) std::cout<<" Start checking selections for the muons in the event :"<<fmuons_nMuons<<std::endl;
    // Need to select events with two isolated muons
    // Need to match one of the isolated muons with trigger muon
    // Need to check if the trigger matched muon has pT>28 GeV
    // Need to build the Z candidate with the trigger matched muon and other muon of atleast 10 GeV and put selections on the Z candidate

    for( int iM = 0; iM<  fmuons_nMuons;iM++){
       double isolationValue = (fmuons_isoCH04[iM] + std::max (0.0, fmuons_isoNH04[iM] + fmuons_isoPhot04[iM] - 0.5*(fmuons_isoPU04[iM]) ) )/ fmuons_pt[iM]; 
       histos->fill1DHist(isolationValue,"muon_isolation_before_selection","","isolation value of muon before any selection","Entries",4,100,0,1.50,1.0,"Test");
       if(debug_new) std::cout<<" muon "<<iM<<" : isolation : "<<isolationValue<<std::endl;
       if(fabs(isolationValue) >0.15) continue;
       fmuons_isomuondzdxy[iM]=true;
		}
    int nIsolatedMuons = 0;
    for(int iM = 0; iM < fmuons_nMuons; iM++) {
      if(fmuons_isomuondzdxy[iM]) {
        nIsolatedMuons++;
      }
    }

		if(nIsolatedMuons>=2){
      Nevents_with_two_isolated_muons++;
		}
		else{
			continue;
		}
    // Fill histogram for number of isolated muons per event
    histos->fill1DHist(nIsolatedMuons, "nIsolatedMuons_per_event", "", "Number of isolated muons per event", "Entries", 4, 10, 0.0, 10.0, 1.0, "Test");
    if(debug_new) std::cout<<" number of isolated muons "<<nIsolatedMuons<<std::endl;

    for( int iM = 0; iM<  fmuons_nMuons;iM++){
		  if(!fmuons_isomuondzdxy[iM]) continue;	
			if(debug_new) std::cout<<" isGlobalmuon :"<<fmuons_isGlobalMuon[iM]<<" : isPFMuon "<<fmuons_isPFMuon[iM]<<" : isTrackerMuon "<<fmuons_isTrackerMuon[iM]<<" : globalTrackNormalizedChi2 "<<fmuons_globalTrackNormalizedChi2[iM]<<" : globalTrackNumberOfValidMuonHits "<<fmuons_globalTrackNumberOfValidMuonHits[iM]<<" : numberOfMatchedStations "<<fmuons_numberOfMatchedStations[iM]<<" : trackerValidPixelHits "<<fmuons_trackerValidPixelHits[iM]<<" : trackerLayers "<<fmuons_trackerLayers[iM]<<std::endl;
       if( fmuons_pt[iM]<10) continue;
       if(fabs(fmuons_dz[iM]) >0.5) continue;
       if(fabs(fmuons_dxy[iM]) >0.2) continue;
       // New conditions
       if(fmuons_isGlobalMuon[iM] ==false) continue;
       if(fmuons_isPFMuon[iM] ==false) continue;
       //if(fmuons_isTrackerMuon[iM] ==false) continue; 
       if(fmuons_globalTrackNormalizedChi2[iM] <0  || fmuons_globalTrackNormalizedChi2[iM] >=10 ) continue;

       if(fmuons_globalTrackNumberOfValidMuonHits[iM] <=0 ) continue;
    	 if(fmuons_numberOfMatchedStations[iM] <=1 ) continue;
       if(fmuons_trackerValidPixelHits[iM] <=0) continue;
       if(fmuons_trackerLayers[iM] <=5 ) continue;
       fmuons_TightId[iM]=true;
       if(debug_new) std::cout<<" muon "<<iM<<" passed all selections and iso bool "<<fmuons_isomuondzdxy[iM]<<std::endl;
    }
    int nTightIsolatedMuons = 0;
    for(int iM = 0; iM < fmuons_nMuons; iM++) {
      if(fmuons_TightId[iM] ) {
        nTightIsolatedMuons++;
      }
    }

    // TO check number of events where we have atleast two isolated muons, this is to check the efficiency of the cut 
    // Count events with at least 2 isolated muons and only events with atleast 2 isolated muons are considered
    // ********************** Isolation check done ****************** 
    if(nTightIsolatedMuons >= 2) {
      Nevents_with_two_tight_isolated_muons++;
    }
    else{
      continue;
    }
    // ********************** Isolation check done ****************** 
    bool event_has_trigger_matched_muon = false;
    if(debug_new) std::cout<<" Isolation checked, now checking trigger matched muons : number of trigger Matched" <<nb_trigger_matched<<std::endl;
    for(int i=0; i<nb_trigger_matched; i++){
    if(debug_new) std::cout<<"pT matched "<<i<<" : "<<pT_matched[i]<<" : eta "<<eta_matched[i]<<" : phi "<<phi_matched[i]<<std::endl;
    }
    for( int iM = 0; iM<  fmuons_nMuons;iM++){
      if(fmuons_TightId[iM] ==false) continue;
      // Now check if the muon matches with trigger , the leading muon in the event
      TLorentzVector mu1; 
      mu1.SetPtEtaPhiM( fmuons_pt[iM], fmuons_eta[iM], fmuons_phi[iM],mass_mu);
      for(int i=0; i<nb_trigger_matched; i++){
        TLorentzVector matched_v; 
        matched_v.SetPtEtaPhiM(pT_matched[i], eta_matched[i], phi_matched[i], mass_mu);
        double deltaR1_value = mu1.DeltaR(matched_v);
        histos->fill1DHist(deltaR1_value,"deltaR_isolated_muon_trigger","","deltaR of isolated muon and trigger muon","Entries",4,100,0,5.0,1.0,"Test");
        if(debug_new) std::cout<<"deltaR1_value for muon "<<iM<<" : "<<deltaR1_value<<" : pT "<<fmuons_pt[iM]<<" : eta "<<fmuons_eta[iM]<<" : phi "<<fmuons_phi[iM]<<" : pT matched "<<pT_matched[i]<<" : eta matched "<<eta_matched[i]<<" : phi matched "<<phi_matched[i]<<std::endl;
        histos->fill1DHist(deltaR1_value,"deltaR_isolated_muon_with_trigger","","deltaR of isolated muon and trigger muon (both muons)","Entries",4,500,0,5,1.0,"Test");
        if(deltaR1_value <0.03) {
            if(debug_new) std::cout<<" yes matched "<<std::endl;
             histos->fill1DHist(deltaR1_value,"deltaR_isolated_muon_trigger_matched","","deltaR of isolated muon and trigger muon which matched","Entries",4,60,0,0.3,1.0,"Test");
            if (fmuons_pt[iM] > pT_cut) {
							double deltapT_value = (pT_matched[i] - fmuons_pt[iM])/fmuons_pt[iM];
              histos->fill1DHist(deltapT_value,"deltapT_isolated_muon_trigger_matched","","|#Delta pT|/pT of isolated muon and trigger muon which matched","Entries",4,100,0,1.0,1.0,"Test");

              if(debug_new) std::cout<<" yes matched pT cut "<<std::endl;
              event_has_trigger_matched_muon = true;
              fmuons_matched_trigger[iM] = true;
              histos->fill1DHist(deltaR1_value,"deltaR_isolated_muon_trigger_matched_pTcut","","deltaR of isolated muon and trigger muon which matched with pT>28 GeV","Entries",4,100,0,0.3,1.0,"Test");
            }
         }// delta R check done
       } // end of trigger matched loop
    } // end of muon loop to check  trigger matched muons
    if(debug_new) std::cout<<" found trigger matched muon with indices "<<std::endl;
    // The matched trigger muon should have pT>28 GeV  
    // This trigger should have pT> 28 GeV
    for(int i=0; i<nb_trigger_matched; i++){
      histos->fill1DHist(pT_matched[i],"pT_trigger_before_matching","","pT of trigger muons before matching","Entries",4,100,0,200,1.0,"Test");
      histos->fill1DHist(eta_matched[i],"eta_trigger_before_matching","","eta of trigger muons before matching","Entries",4,100,-2.5, 2.5,1.0,"Test");
      histos->fill1DHist(phi_matched[i],"phi_trigger_before_matching","","phi of trigger muons before matching","Entries",4,100,-3.5,3.5,1.0,"Test");
     }

    // ********************** Trigger matched check done ****************** 
    if (!event_has_trigger_matched_muon) {
      if(debug_new) std::cout<<" no trigger matched muon found "<<std::endl;
      Nevents_not_trigger_matched++;
      continue;
    } else {
    Nevents_trigger_matched++;
    }
     // ********************** Trigger matched check done ****************** 
    // Plotting pT, eta, phi for muons which mattched the trigger and also pT, eta, phi for trigger muons which matched
    // One can ignore this sanity check plots
    for(int iM=0; iM< fmuons_nMuons; iM++){
      if(fmuons_matched_trigger[iM] ==true){
        histos->fill1DHist(fmuons_pt[iM],"pT_matched_trigger","","pT of trigger matched muons","Entries",4,100,0,200,1.0,"Test");
        histos->fill1DHist(fmuons_eta[iM],"eta_matched_trigger","","eta of trigger matched muons","Entries",4,100,-2.5,2.5,1.0,"Test");
        histos->fill1DHist(fmuons_phi[iM],"phi_matched_trigger","","phi of trigger matched muons","Entries",4,100,-3.5,3.5,1.0,"Test");
      }
    }
    for(int i=0; i<nb_trigger_matched; i++){
      if(pT_matched[i] <pT_cut) continue;
      histos->fill1DHist(pT_matched[i],"pT_trigger_after_matching","","pT of trigger muons after matching","Entries",4,100,0,200,1.0,"Test");
      histos->fill1DHist(eta_matched[i],"eta_trigger_after_matching","","eta of trigger muons after matching","Entries",4,100,-2.5, 2.5,1.0,"Test");
      histos->fill1DHist(phi_matched[i],"phi_trigger_after_matching","","phi of trigger muons after matching","Entries",4,100,-3.5,3.5,1.0,"Test");
     }
    // *********************** Zmuon building done ****************** 
    // Now we build the Z candidate with the trigger matched muon and other muon of atleast 10 GeV and put selections on the Z candidate
     for(int iM=0 ; iM< fmuons_nMuons; iM++){
        if(fmuons_isomuondzdxy[iM] ==false) continue;
        if(fmuons_TightId[iM] ==false) continue;
        for(int jM=iM+1; jM< fmuons_nMuons; jM++){
            if(fmuons_isomuondzdxy[jM] ==false) continue;
            if(fmuons_TightId[jM] ==false) continue;
             if(fmuons_charge[iM] ==fmuons_charge[jM]) continue;

             if(debug_new) std::cout<<" checking muon pair "<<iM<<" : "<<jM<<" trigger matching "<<fmuons_matched_trigger[iM]<<" : "<<fmuons_matched_trigger[jM]<<std::endl;
             if(fmuons_matched_trigger[iM] ==false && fmuons_matched_trigger[jM] ==false) continue;
             TLorentzVector mu1;
             TLorentzVector mu2;
             mu1.SetPtEtaPhiM( fmuons_pt[iM], fmuons_eta[iM], fmuons_phi[iM],mass_mu);
             mu2.SetPtEtaPhiM( fmuons_pt[jM], fmuons_eta[jM], fmuons_phi[jM],mass_mu);
             mass = (mu1+mu2).Mag();
 
	          if(mass>=70&&mass<110){ 
               double mass_diff = abs(mass - nominal_mass);
               nZcandidates++;
               // The Z pair is only acceptable when the mass difference is less than 10 GeV
               if(abs(mass_diff) > 10) continue;
               nZcandidates_narrow++;
               // The Z pair is only acceptable when the mass difference is less than 10 GeV
					 	  // In case we already had a possible Z canddiate, and now we found another Z candidate, ie. multiple Z candidate case
               if(mass_diff < best_mass_diff){  
                best_mass_diff = mass_diff;
                first_index = iM;
                second_index = jM;
                z_mass = mass;
                z_pt = (mu1 + mu2).Pt();
                z_eta = (mu1 + mu2).Eta();
                z_phi = (mu1 + mu2).Phi();

                passZmumusel = true;
                isolation_value_one = (fmuons_isoCH03[iM] + std::max (0.0, fmuons_isoNH03[iM] + fmuons_isoPhot03[iM] - 0.5*(fmuons_isoPU03[iM]) ) )/ fmuons_pt[iM]; 
                isolation_value_two = (fmuons_isoCH03[jM] + std::max (0.0, fmuons_isoNH03[jM] + fmuons_isoPhot03[jM] - 0.5*(fmuons_isoPU03[jM]) ) )/ fmuons_pt[jM]; 
                iso_PF_first = isolation_value_one;  
                iso_PF_second = isolation_value_two;

			          isolation1 = fabs(isolation_value_one);
			          isolation2 = fabs(isolation_value_two);
                if(debug_new) std::cout<<" best mass diff "<<mass_diff<<" : first index "<<first_index<<" : second index "<<second_index<<" : z mass "<<z_mass<<" : z pt "<<z_pt<<" : z eta "<<z_eta<<" : z phi "<<z_phi<<std::endl;
               }
              }// end of mass selection
             if(debug_new) std::cout<<"event passed muons "<<iM<<" : "<<jM<<" mass "<<z_mass<<" number of muons "<<fmuons_nMuons<<"mass Z "<<z_mass<<" : "<<fmuons_globalTrackNormalizedChi2[iM]<<" : "<<fmuons_globalTrackNormalizedChi2[jM]<<std::endl;
         }// end of muon pair loop
       }// end of muon loop
      // To check the efficiency of the 70-110 GeV cut
      if (nZcandidates == 1 || nZcandidates == 2) passed_Zevents_full_range++;
      else { not_passed_Zevents_full_range++;}

      // Important
      // To check the efficiency of the cut of 10 GeV on mass window and know which events to keep
      if (nZcandidates_narrow == 1 || nZcandidates_narrow == 2) {
				 if(nZcandidates_narrow==1) passed_Zevents_one++;
				 else if(nZcandidates_narrow==2) passed_Zevents_two++;
				passed_Zevents++;
			}
      else { 
        if(nZcandidates_narrow==0) not_passed_Zevents_zero++; 
        else not_passed_Zevents_multiple++;  
        not_passed_Zevents++;
        passZmumusel = false;
        }
      if(debug_new) std::cout<<" number of Z candidates "<<nZcandidates_narrow<<std::endl;
			if(passZmumusel ==false) continue;
      if(debug_new) std::cout<<" event passed all selections "<<std::endl;
      // *********************** Zmuon selection done ****************** 
       // More checks to make sure that the muon indices are valid before extracting the individual muons corresponding to the indices
       if (first_index < fmuons_nMuons && second_index < fmuons_nMuons &&
           first_index < fmuons_nMuons && second_index < fmuons_nMuons  &&
           first_index < fmuons_nMuons && second_index < fmuons_nMuons ) { 
           valid_muon_indices++;
        }
        else {
          if(debug_new) std::cout<<"Muon indices are out of bounds: first_index="<<first_index<<", second_index="<<second_index<<std::endl;
          non_valid_muon_indices++;
          continue;
        }
        if(debug_new) std::cout<<" number of valid muon indices "<<valid_muon_indices<<std::endl;
        if(debug_new) std::cout<<" number of non valid muon indices "<<non_valid_muon_indices<<std::endl;

       // Check if both muons pass the isolation and dxy cuts  : redundant sanity check
			 if(fmuons_isomuondzdxy[first_index] ==false) continue;
			 if(fmuons_isomuondzdxy[second_index] ==false) continue;
			 if(fmuons_TightId[first_index] ==false) continue;
			 if(fmuons_TightId[second_index] ==false) continue;

       fmuons_Zcand[first_index]=true; 
       fmuons_Zcand[second_index]=true; 

      for(int tM=0; tM< fmuons_nMuons;tM++){
          if(fmuons_Zcand[tM]==true && fmuons_isomuondzdxy[tM]==true && fmuons_TightId[tM]==true)  
           histos->fill1DHist(fmuons_pt[tM],"muon_pT_events_from_Z","","muon pT for those muons which passes Z selection ","Entries",4,100,0,200,1.0,"Test");
      }// end of for loop iterating over all muons to fill muon pT andz_mass

    histos->fill1DHist(z_mass,"Zmass","","mass : Z","Entries/(0.4GeV)",4,100,70,110.0,1.0,"Test");
    histos->fill1DHist(z_pt,"Zpt","","pT : Z","Entries",4,100,0,200.0,1.0,"Test");
    histos->fill1DHist(z_phi,"Zphi","","#phi : Z","Entries",4,50,-pi,pi,1.0,"Test");
    histos->fill1DHist(z_eta,"Zeta","","#eta : Z","Entries",4,100,-2.5,2.5,1.0,"Test");

    if(runnb_previous_event != fRun ||   lumis_previous_event !=  fLumiSect)   _instlumi =instlumi(fRun, fLumiSect, year) ;
		float intlumi_to_add; 
     if(runnb_previous_event != fRun )
			{ 
				if(year=="2016") {
					intlumi_to_add  = 0;
					_integratelumi = integlumi_2016(fRun) + intlumi_to_add;
				}
				else if(year=="2017") {
					intlumi_to_add = 39.32673126400002;
				 	_integratelumi = (integlumi_2017(fRun)/1000.0)+intlumi_to_add;
				}
				else if(year=="2018") {
					 intlumi_to_add = 39.32673126400002 + 44.52667700172356; 
					_integratelumi = integlumi_2018(fRun)+ intlumi_to_add;
				}
			}
              
     if(debug_bool) std::cout<<" going for the event   "<<fRun<<std::endl; 
     runnb_previous_event =fRun; 
     lumis_previous_event =  fLumiSect ;
     // get Run, Event, Start, End
     Int_t key_run=(Int_t)fRun;
     if(m_RunEvent.find(key_run) == m_RunEvent.end()) {
       m_RunEvent[key_run]=0;
       m_Run_usedevents[key_run]=0;
       m_Run_usedhits[key_run]=0;
       // get max number, it should be 4294967295 for unsigned int
       m_RunStart[key_run]=std::numeric_limits<unsigned int>::max();       
       m_RunEnd[key_run]=0;
     }
     m_RunEvent[key_run]=m_RunEvent[key_run]+1;
     if(ftimeSecond>m_RunEnd[key_run]) m_RunEnd[key_run]=ftimeSecond;
     if(ftimeSecond<m_RunStart[key_run]) m_RunStart[key_run]=ftimeSecond;
     //************************************************************************
     // Check the presence of needed and not empty vectors
     //************************************************************************
     Int_t recsegtrk=0;
     if(frecHits2D_nRecHits2D > 0) recsegtrk=recsegtrk+1;
     if(fcscSegments_recHitRecord_endcap->size() > 0) recsegtrk=recsegtrk+2;
     if(fmuons_cscSegmentRecord_nRecHits->size() > 0) {
       Int_t ncscsegm=0;
       for(UInt_t i=0;i<fmuons_cscSegmentRecord_nRecHits->size();i++) 
	   if((*fmuons_cscSegmentRecord_nRecHits)[i].size() > 0) ncscsegm++; 
       if(ncscsegm > 0) recsegtrk=recsegtrk+4;
     }
     histos->fill1DHist((Float_t)recsegtrk,"recsegtrk","","Bit 1-RecHits2D, Bit 2-Segments and Bit 3-Tracks per event","Entries",4,8,0.0,8.0,1.0,"Test");
     if(recsegtrk==7) { // Analyse event only when all 3 vectors not empty //Laurent: this ensures there's at least 1 rechit, 1 segment and 1 muon associated to them (??)
     if(ient<flag_print) cout<<endl<<"*** "<<ient+1<< " Event "<<fEvent<<endl;

     //*****************************************************************
     //  Treat ME1/4 differently (due 3 strips ganging),
     //  using only frecHits2D_nRecHits2D;
     //  fill maps m_nRecHitlayer,m_nRecHitchamber (# of hits per layer
     //  and chamber)
     //******************************************************************

      //     GetME14RecHits(histos);//Commented out  by laurent: now treating ME1/4 (a.k.a ME11a) as other station/rings

     //************************************************************************
     // Cycling thru segments in fcscSegments_recHitRecord_*, 
     // put their rechits X,Y local into 
     // maps m_cscSegments_recHitRecordX,*Y[key_segment][0-5], 0-5 are layers
     // and count # of segments per chamber in m_nsegments_chamber[chamber]
     //************************************************************************

     GetSegments(histos);

     //*******************************************************************
     // Cycling thru tracks in event and segments in them, put to map  
     // m_cscSegments_single_trk_recHitRecord[key_layer][0-2]
     //corresponding rechits
     //*******************************************************************

     GetTracks(histos);

     //**************************************
     //*** cycling over recHits2D_nRecHits to find SumQ and put it into
     //*** map m_cscSegments_single_trk_recHitRecord[key_layer][0-2]
     //***************************************

    GetRecHitsSumQ(histos);

     //**********************************************************************
     // Add m_RecHitlayerME14_final to map 
     // m_cscSegments_single_trk_recHitRecord_final
     //**********************************************************************

     //     AddME14RecHits(); Commented out Laurent
     if(debug_bool) std::cout<<"collected charges and going further "<<std::endl;
		 if(debug_bool) std::cout<<" single trk size "<<m_cscSegments_single_trk_recHitRecord_final.size()<<std::endl;
      if(m_cscSegments_single_trk_recHitRecord_final.size() > 0) {
         if(debug_bool) std::cout<<"entered here in single track  "<<std::endl;				
         histos->fill1DHist((Float_t)m_cscSegments_single_trk_recHitRecord_final.size(),"final_used_rechits_per_event","","Final number of used rechits per event","Entries",4,100,0.0,100.0,1.0,"Test");


         if(debug_bool) std::cout<<"filled the single track histogram  "<<std::endl;				
         m_Run_usedevents[key_run]=m_Run_usedevents[key_run]+1;
         m_Run_usedhits[key_run]=m_Run_usedhits[key_run]+(Int_t)m_cscSegments_single_trk_recHitRecord_final.size();
      }

     //**********************************************************************
     // Cycling thru  m_cscSegments_single_trk_recHitRecord_final to SumQ
     // histograms in HV segments
     //**********************************************************************

		 if(debug_bool) std::cout<<" histogram in HV segemnts "<<std::endl;
     if(m_cscSegments_single_trk_recHitRecord_final.size() > 0) 
      if(debug_bool) std::cout<<"filling histos now : entry "<<_eventNb<<std::endl;
		 if(debug_bool_region) std::cout<<"event is there and filling the charge now"<<std::endl;
        FillSumQHists(histos); 


     } // end of if(recsegtrk==7)
  } // end of  for(Int_t ient=0;
  std::cout<<" passed Events m"<<passed_Zevents<<std::endl;
 
  if(debug_bool) std::cout<<"end of entry loop and reseting tree branches now : entry "<<_eventNb<<std::endl;
  tree->ResetBranchAddresses();

  //************************************************************************
  // End of tree analysis printout
  //**************************************************************************
  cout<<" going to print out tree details"<<std::endl;
  Int_t cnt=0,cnt_readin=0,cnt_used=0,cnt_usedhits=0;
  for(std::map<Int_t,Int_t>::iterator It=m_RunEvent.begin(); It!= m_RunEvent.end();++It) {
     Int_t key=(*It).first;
     cnt++;     
     Int_t dur=(Int_t)(m_RunEnd[key]-m_RunStart[key]);
     cnt_readin=cnt_readin+m_RunEvent[key];
     cnt_used=cnt_used+m_Run_usedevents[key];
     cnt_usedhits=cnt_usedhits+m_Run_usedhits[key];
     cout<<cnt<<" Run  Events  Start  End  Duration  Used events  Used hits "<<key<<" "<<(*It).second<<" "<<m_RunStart[key]<<" "<<m_RunEnd[key]<<" "<<dur<<" "<<m_Run_usedevents[key]<<" "<<m_Run_usedhits[key]<<endl;
       }
  cout<<"--------------------------------------------------"<<endl;
	std::cout<<" Valid muon indices "<<valid_muon_indices<<std::endl;
	std::cout<<" Non valid muon indices "<<non_valid_muon_indices<<std::endl;

  std::cout<<" total number of events "<<nentries<<std::endl;
  std::cout<<" total  isolated events "<<Nevents_with_two_isolated_muons<<std::endl;
  std::cout<<" total  isolated events "<<Nevents_with_two_tight_isolated_muons<<std::endl;
  std::cout<<" total trigger matched events "<<Nevents_trigger_matched<<std::endl;
  std::cout<<" total not trigger matched events "<<Nevents_not_trigger_matched<<std::endl;
  std::cout<<" total not trigger matched events by pT cut "<<Nevents_not_trigger_matched_pTcut<<std::endl;

	std::cout<<" total Z events  "<<passed_Zevents<<std::endl;
	std::cout<<" total Z events  with 1 candidate"<<passed_Zevents_one<<std::endl;
	std::cout<<" total Z events  with 2 candidate"<<passed_Zevents_two<<std::endl;
	std::cout<<" total not Z events  "<<not_passed_Zevents<<std::endl;
	std::cout<<" total not Z events with zero Z candidates  "<<not_passed_Zevents_zero<<std::endl;
	std::cout<<" total not Z events with multiple Z candidates  "<<not_passed_Zevents_multiple<<std::endl;
	std::cout<<" total Z events wider range "<<passed_Zevents_full_range<<std::endl;
	std::cout<<" total not Z events wider range "<<not_passed_Zevents_full_range<<std::endl;

  cout<<"Total used hits "<<cnt_usedhits<<" in "<<cnt_used<<" used events  from "<<cnt_readin<<" read in events in "<<m_RunEvent.size()<<" runs"<<endl;
  cout<<endl;
  cout<<" End of AnalysisGasGain::CycleTree "<<endl;
///
  //*************************************************************************
  // write and close output file with hists always first in case if hists
  // were stored in f directory
  //************************************************************************


  histos->writeHists(histrootfile);
	
  if(debug_bool) std::cout<<" before closing the histogram and after writing the histogram "<<endl;
  histrootfile->Close();

  if(debug_bool) std::cout<<" issue in closing  "<<endl;
  f->Close();
}


