// -*- C++ -*-
//
// Package:    MonteCarloInfo/WeightAnalyzer
// Class:      WeightAnalyzer
// 
/**\class WeightAnalyzer WeightAnalyzer.cc MonteCarloInfo/WeightAnalyzer/plugins/WeightAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  clint richardson
//         Created:  Fri, 11 Sep 2015 16:29:00 GMT
//
//


// system include files
#include <memory>
#include <iostream>
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "TLorentzVector.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "JetMETCorrections/Modules/interface/JetResolution.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "TMath.h"
#include "TH1.h"
#include "TH2D.h"
#include <TRandom.h>
#include <TRandom3.h>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <utility> // std::pair

//
// class declaration
//

class WeightAnalyzer : public edm::EDAnalyzer {
public:
  explicit WeightAnalyzer(const edm::ParameterSet&);
  ~WeightAnalyzer();
  
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  
  
private:
  virtual void beginJob() override;
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override;
  TH1D* weightHist;

  TH1D* NumTrueHist;
  TH1D* BEff_Dptbins_b;   
  TH1D* BEff_Dptbins_c;   
  TH1D* BEff_Dptbins_udsg;
  TH1D* BEff_Nptbins_b;  
  TH1D* BEff_Nptbins_c;   
  TH1D* BEff_Nptbins_udsg;
  TH1D* BEffLoose_Nptbins_b;  
  TH1D* BEffLoose_Nptbins_c;   
  TH1D* BEffLoose_Nptbins_udsg;
  TH2D* WEff_Dpteta;
  TH2D* WEff_Npteta;
  TH1D* WEff_DptbinsMatch;   
  TH1D* WEff_DptbinsMass;   
  TH1D* WEff_NptbinsMass;  
  TH1D* WEff_NptbinsPN;  
  TH1D* TEff_DptbinsMatch;   
  TH1D* TEff_DptbinsMass;   
  TH1D* TEff_NptbinsMass;  
  TH1D* TEff_NptbinsPN;  
  TH1D* HEff_D;
  TH1D* HEff_N;
  TH1D* HEff1b_Dptbins;
  TH1D* HEff1b_Nptbins;
  TH1D* HEff2b_Dptbins;
  TH1D* HEff2b_Nptbins;
  //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
  //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
  //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
  //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
  
  // ----------member data ---------------------------
  const edm::InputTag jetsTag;
  const edm::InputTag jetsTagAK4;
  const edm::InputTag genParticles_it;
  const std::string   discriminatorTag;
  const double  discriminatorValue;
  const int     ptNBins;
  const double  ptMin;
  const double  ptMax;
  const int     etaNBins;
  const double  etaMin;
  const double  etaMax;
  std::string MCL3;
  std::string MCL2;
  std::string MCL1;
  std::string SF;
  std::string RES;
  FactorizedJetCorrector *jecak8;
  FactorizedJetCorrector *jecak8_all;
  JetCorrectorParameters *L3JetParAK8;
  JetCorrectorParameters *L2JetParAK8;
  JetCorrectorParameters *L1JetParAK8;
  JME::JetResolution resolutionAK8;
  JME::JetResolutionScaleFactor resolution_SF;
  Variation JERsystematic;
  TRandom3 *gRandom;

  edm::EDGetTokenT<GenEventInfoProduct> GEIPtoken;  
  edm::EDGetTokenT<std::vector<pat::Jet>> JETtoken;  
  edm::EDGetTokenT<std::vector<pat::Jet>> JETtokenAK4;  
  edm::EDGetTokenT<double> RHOtoken;
  edm::EDGetTokenT<reco::GenParticleCollection> GPtoken;
  edm::EDGetTokenT<std::vector<PileupSummaryInfo>> APItoken;

};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
WeightAnalyzer::WeightAnalyzer(const edm::ParameterSet& iConfig):
  jetsTag(iConfig.getParameter<edm::InputTag>("JetsTagAK8")),
  jetsTagAK4(iConfig.getParameter<edm::InputTag>("JetsTag")),
  genParticles_it(iConfig.getParameter<edm::InputTag>("genParticles_it")),
  discriminatorTag(iConfig.getParameter<std::string>("DiscriminatorTag")),
  discriminatorValue(iConfig.getParameter<double>("DiscriminatorValue")),
  ptNBins(iConfig.getParameter<int>("PtNBins")),
  ptMin(iConfig.getParameter<double>("PtMin")),
  ptMax(iConfig.getParameter<double>("PtMax")),
  etaNBins(iConfig.getParameter<int>("EtaNBins")),
  etaMin(iConfig.getParameter<double>("EtaMin")),
  etaMax(iConfig.getParameter<double>("EtaMax"))
{
   //now do what ever initialization is needed
  edm::Service<TFileService> fs;
  weightHist = fs->make<TH1D>("weightHist","MC Weight",10,-2,2); weightHist->Sumw2();
  NumTrueHist = fs->make<TH1D>("NumTrueHist","N True Interactions",100,0,100); NumTrueHist->Sumw2();

   WEff_Dpteta = fs->make<TH2D>("WEff_Dpteta","",ptNBins,ptMin,ptMax,etaNBins,etaMin,etaMax); WEff_Dpteta->Sumw2();
   WEff_Npteta = fs->make<TH2D>("WEff_Npteta","",ptNBins,ptMin,ptMax,etaNBins,etaMin,etaMax); WEff_Npteta->Sumw2();
  
   double ptbins[16] = {175,200,250,300,350,400,450,500,550,600,700,800,1000,1200,1600,2000};
   WEff_DptbinsMatch = fs->make<TH1D>("WEff_DptbinsMatch","",15,ptbins); WEff_DptbinsMatch->Sumw2();
   WEff_DptbinsMass = fs->make<TH1D>("WEff_DptbinsMass","",15,ptbins); WEff_DptbinsMass->Sumw2();
   WEff_NptbinsPN = fs->make<TH1D>("WEff_NptbinsPN","",15,ptbins); WEff_NptbinsPN->Sumw2();
   TEff_DptbinsMatch = fs->make<TH1D>("TEff_DptbinsMatch","",15,ptbins); TEff_DptbinsMatch->Sumw2();
   TEff_DptbinsMass = fs->make<TH1D>("TEff_DptbinsMass","",15,ptbins); TEff_DptbinsMass->Sumw2();
   TEff_NptbinsPN = fs->make<TH1D>("TEff_NptbinsPN","",15,ptbins); TEff_NptbinsPN->Sumw2();

  double ptbinsB[15] = {0,30,50,70,100,140,200,300,400,500,600,800,1000,1200,2000};
  BEff_Dptbins_b    = fs->make<TH1D>("BEff_Dptbins_b   ","",14,ptbinsB); BEff_Dptbins_b->Sumw2();
  BEff_Dptbins_c    = fs->make<TH1D>("BEff_Dptbins_c   ","",14,ptbinsB); BEff_Dptbins_c->Sumw2();
  BEff_Dptbins_udsg = fs->make<TH1D>("BEff_Dptbins_udsg","",14,ptbinsB); BEff_Dptbins_udsg->Sumw2();
  BEff_Nptbins_b      = fs->make<TH1D>("BEff_Nptbins_b     ","",14,ptbinsB); BEff_Nptbins_b->Sumw2();
  BEff_Nptbins_c      = fs->make<TH1D>("BEff_Nptbins_c     ","",14,ptbinsB); BEff_Nptbins_c->Sumw2();
  BEff_Nptbins_udsg   = fs->make<TH1D>("BEff_Nptbins_udsg  ","",14,ptbinsB); BEff_Nptbins_udsg->Sumw2();
  BEffLoose_Nptbins_b      = fs->make<TH1D>("BEffLoose_Nptbins_b     ","",14,ptbinsB); BEffLoose_Nptbins_b->Sumw2();
  BEffLoose_Nptbins_c      = fs->make<TH1D>("BEffLoose_Nptbins_c     ","",14,ptbinsB); BEffLoose_Nptbins_c->Sumw2();
  BEffLoose_Nptbins_udsg   = fs->make<TH1D>("BEffLoose_Nptbins_udsg  ","",14,ptbinsB); BEffLoose_Nptbins_udsg->Sumw2();

   double ptbinsH[14] = {250,300,350,400,450,500,550,600,700,800,1000,1200,1600,2000};
   HEff_D = fs->make<TH1D>("HEff_D","",1,0,2000); HEff_D->Sumw2();
   HEff_N = fs->make<TH1D>("HEff_N","",1,0,2000); HEff_N->Sumw2();
   HEff1b_Dptbins = fs->make<TH1D>("HEff1b_Dptbins","",13,ptbinsH); HEff1b_Dptbins->Sumw2();
   HEff1b_Nptbins = fs->make<TH1D>("HEff1b_Nptbins","",13,ptbinsH); HEff1b_Nptbins->Sumw2();
   HEff2b_Dptbins = fs->make<TH1D>("HEff2b_Dptbins","",13,ptbinsH); HEff2b_Dptbins->Sumw2();
   HEff2b_Nptbins = fs->make<TH1D>("HEff2b_Nptbins","",13,ptbinsH); HEff2b_Nptbins->Sumw2();
 

//for 2017:Summer19UL17_V5/
   MCL1 = "/home/fsimpson/CMSSW_10_2_10/src/data/Summer19UL17_V5/Summer19UL17_V5_MC_L1FastJet_AK8PFPuppi.txt";
   MCL2 = "/home/fsimpson/CMSSW_10_2_10/src/data/Summer19UL17_V5/Summer19UL17_V5_MC_L2Relative_AK8PFPuppi.txt";
   MCL3 = "/home/fsimpson/CMSSW_10_2_10/src/data/Summer19UL17_V5/Summer19UL17_V5_MC_L3Absolute_AK8PFPuppi.txt";
   SF = "/home/fsimpson/CMSSW_10_2_10/src/data/Summer19UL17_JRV3/Summer19UL17_JRV3_MC_SF_AK4PFchs.txt"; //no SF for Puppi available on FWLJMET
   RES = "/home/fsimpson/CMSSW_10_2_10/src/data/Summer19UL17_JRV3/Summer19UL17_JRV3_MC_PtResolution_AK8PFPuppi.txt";


   L1JetParAK8  = new JetCorrectorParameters(MCL1);
   L2JetParAK8  = new JetCorrectorParameters(MCL2);
   L3JetParAK8  = new JetCorrectorParameters(MCL3);

   std::vector<JetCorrectorParameters> vParAK8;
   std::vector<JetCorrectorParameters> vParAK8_all;

   vParAK8.push_back(*L2JetParAK8);
   vParAK8.push_back(*L3JetParAK8);

   vParAK8_all.push_back(*L1JetParAK8);
   vParAK8_all.push_back(*L2JetParAK8);
   vParAK8_all.push_back(*L3JetParAK8);

   jecak8 = new FactorizedJetCorrector(vParAK8);
   jecak8_all = new FactorizedJetCorrector(vParAK8_all);

   resolutionAK8 = JME::JetResolution(RES);
   resolution_SF = JME::JetResolutionScaleFactor(SF);
   JERsystematic = Variation::NOMINAL;    

   gRandom = new TRandom3();

  edm::InputTag GEIPtag("generator");
  GEIPtoken = consumes<GenEventInfoProduct>(GEIPtag);
  JETtoken = consumes<std::vector<pat::Jet>>(jetsTag);
  JETtokenAK4 = consumes<std::vector<pat::Jet>>(jetsTagAK4);
  GPtoken = consumes<reco::GenParticleCollection>(genParticles_it);
  edm::InputTag rhoSrc_("fixedGridRhoFastjetAll", "");
  RHOtoken = consumes<double>(rhoSrc_);
  edm::InputTag APItag("slimmedAddPileupInfo");
  APItoken = consumes<std::vector<PileupSummaryInfo>>(APItag);
    

}


WeightAnalyzer::~WeightAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

  delete jecak8;
  delete jecak8_all;
  delete L1JetParAK8;
  delete L2JetParAK8;
  delete L3JetParAK8;
  delete gRandom;

}


//
// member functions
//

// ------------ method called for each event  ------------
void
WeightAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  using namespace edm;
  edm::Handle<GenEventInfoProduct> genEvtInfo;
  iEvent.getByToken(GEIPtoken, genEvtInfo);
  
  double weight = 1.0;
  if(genEvtInfo->weight() < 0) weight = -1;
  weightHist->Fill(weight);


  edm::Handle<std::vector<pat::Jet> > jetsAK4;
  iEvent.getByToken(JETtokenAK4,jetsAK4);


  // Pileup
  edm::Handle<std::vector<PileupSummaryInfo>> PupInfo;
  iEvent.getByToken(APItoken, PupInfo);


  int NumTrueInts = -1;
  for(std::vector<PileupSummaryInfo>::const_iterator PVI = PupInfo->begin(); PVI != PupInfo->end(); ++PVI){
    int BX = PVI->getBunchCrossing();
    if(BX == 0) NumTrueInts = PVI->getTrueNumInteractions();
  }
  NumTrueHist->Fill(NumTrueInts,weight);

  // loop over jets
  for(std::vector<pat::Jet>::const_iterator it = jetsAK4->begin(); it != jetsAK4->end(); ++it){
    
    int hadronFlavor = it->hadronFlavour();
    if( abs(hadronFlavor)==5 ){
      BEff_Dptbins_b->Fill(it->pt());
      
      //2017 UL values https://btv-wiki.docs.cern.ch/ScaleFactors/UL2017/ 
      double disc = it->bDiscriminator("pfDeepFlavourJetTags:probb") + it->bDiscriminator("pfDeepFlavourJetTags:probbb") + it->bDiscriminator("pfDeepFlavourJetTags:problepb");
      if( disc > 0.3040) BEff_Nptbins_b->Fill(it->pt());
      if( disc > 0.0532) BEffLoose_Nptbins_b->Fill(it->pt()); 
      
    }
    else if( abs(hadronFlavor)==4 ){
      BEff_Dptbins_c->Fill(it->pt());

      double disc = it->bDiscriminator("pfDeepFlavourJetTags:probb") + it->bDiscriminator("pfDeepFlavourJetTags:probbb") + it->bDiscriminator("pfDeepFlavourJetTags:problepb");
              
      if( disc > 0.3040) BEff_Nptbins_c->Fill(it->pt());
      if( disc > 0.0532) BEffLoose_Nptbins_c->Fill(it->pt()); 
    }
    else{
      BEff_Dptbins_udsg->Fill(it->pt());
      
      double disc = it->bDiscriminator("pfDeepFlavourJetTags:probb") + it->bDiscriminator("pfDeepFlavourJetTags:probbb") + it->bDiscriminator("pfDeepFlavourJetTags:problepb");
      if( disc > 0.3040) BEff_Nptbins_udsg->Fill(it->pt());
      if( disc > 0.0532) BEffLoose_Nptbins_udsg->Fill(it->pt());
    }
  }

   edm::Handle<std::vector<pat::Jet> > jets;
   iEvent.getByToken(JETtoken,jets);
  
   std::vector<double> jetPt;
   std::vector<double> jetEta;
   std::vector<double> jetPhi;
   std::vector<double> jetEnergy;
   std::vector<double> jetPM;
   std::vector<int>   PNWtag;
   std::vector<int>   PNTtag;
   std::vector<int>   SubSDcsv1bTag;
   std::vector<int>   SubSDcsv2bTag;
  
   // loop over jets
   for(std::vector<pat::Jet>::const_iterator ijet = jets->begin(); ijet != jets->end(); ++ijet){
    
 
   pat::Jet jet = *ijet;
   pat::Jet corrak8 = jet.correctedJet(0);
   double pt = corrak8.pt();

   if(pt<175) continue; //low pT jets saved since 2017 throw an error: "jet is not a PFjet"
  
    bool tightJetID = false;

   //Updated for 2017/18UL 106X https://twiki.cern.ch/twiki/bin/view/CMS/JetID13TeVUL
    if(abs(corrak8.eta()) <= 3.0){
       tightJetID = (abs(corrak8.eta()) > 2.7 && corrak8.neutralHadronEnergyFraction() < 0.9999) || 
       (abs(corrak8.eta()) <= 2.7 && abs(corrak8.eta()) > 2.6 && corrak8.neutralHadronEnergyFraction() < 0.90 && 
   		corrak8.neutralEmEnergyFraction() < 0.99) || 
   	   (abs(corrak8.eta()) <= 2.6 && 
      corrak8.chargedHadronEnergyFraction() > 0 && corrak8.chargedMultiplicity() > 0 && corrak8.neutralEmEnergyFraction() < 0.90 && 
      (corrak8.chargedMultiplicity()+corrak8.neutralMultiplicity()) > 1 && corrak8.neutralHadronEnergyFraction() < 0.90);
     }else{
        tightJetID = abs(corrak8.eta()) <= 5.0 && corrak8.neutralEmEnergyFraction() < 0.9 && corrak8.neutralMultiplicity() > 2;
     }
     
    if(!tightJetID) continue;

     ////////////// First make an L1-L2-L3 corrected AK8 jet for kinematics //////////////

       double ptscale = 1.0;
       double correction = 1.0;
       double pt_raw = jet.correctedJet(0).pt();

       edm::Handle<double> rhoHandle;
       iEvent.getByToken(RHOtoken, rhoHandle);
       double rho = std::max(*(rhoHandle.product()), 0.0);   

       jecak8_all->setJetEta(jet.eta());
       jecak8_all->setJetPt(pt_raw);
       jecak8_all->setJetA(jet.jetArea());
       jecak8_all->setRho(rho); 
    
       try{
         correction = jecak8_all->getCorrection();
       }
       catch(...){
         std::cout << "WARNING! Exception thrown by JetCorrectionUncertainty!" << std::endl;
         std::cout << "WARNING! Possibly, trying to correct a jet/MET outside correction range." << std::endl;
         std::cout << "WARNING! Jet/MET will remain uncorrected." << std::endl;
       }
    
       corrak8.scaleEnergy(correction);
       pt = corrak8.pt(); // scaled pT
     
       JME::JetParameters parameters;
       parameters.setJetPt(pt);
       parameters.setJetEta(corrak8.eta());
       parameters.setRho(rho);
       double res = resolutionAK8.getResolution(parameters);
       double factor = resolution_SF.getScaleFactor(parameters,JERsystematic) - 1;


      //Energy is scaled here??
   //    const reco::GenJet * genJet = jet.genJet();
   //    bool smeared = false;
   //    if(genJet){
   //      double deltaPt = fabs(genJet->pt() - pt);
   //      double deltaR = reco::deltaR(genJet->p4(),corrak8.p4());	
   //      if (deltaR < 0.4 && deltaPt <= 3*pt*res){
   //  	  double gen_pt = genJet->pt();
   //  	  double reco_pt = pt;
   //  	  double deltapt = (reco_pt - gen_pt) * factor;
   //  	  ptscale = std::max(0.0, (reco_pt + deltapt) / reco_pt);
   //  	  smeared = true;
   //      }
   //    }
   //    if (!smeared && factor>0) {
   //      gRandom->SetSeed(abs(static_cast<int>(jet.phi()*1e4)));
   //      ptscale = std::max(0.0, gRandom->Gaus(pt,sqrt(factor*(factor+2))*res*pt)/pt);
   //    }

  //    corrak8.scaleEnergy(ptscale);

       if(fabs(corrak8.eta()) > 2.4) continue;

       jetPt.push_back(corrak8.pt());
       jetEta.push_back(corrak8.eta());
       jetPhi.push_back(corrak8.phi());
       jetEnergy.push_back(corrak8.energy());
    
       /////////////// Now make an L2+L3 corrected AK8 jet for grooming ///////////////////////
//       pat::Jet l2l3jet = *ijet;
//       jecak8->setJetEta(l2l3jet.correctedJet(0).eta());
//       jecak8->setJetPt(l2l3jet.correctedJet(0).pt());
//       jecak8->setJetE(l2l3jet.correctedJet(0).energy());
//       jecak8->setJetA(l2l3jet.jetArea());
//       jecak8->setRho(rho);
//       double corr = jecak8->getCorrection();
//       l2l3jet.scaleEnergy(corr);
//
//       parameters.setJetPt(l2l3jet.pt());
//       parameters.setJetEta(l2l3jet.eta());
//       parameters.setRho(rho);
//       res = resolutionAK8.getResolution(parameters);

       // JetWTagging TWiki: 80X JER, resolution scale factor = 1.26 (2017), 1.114 (2018)
       //change to UL!
//       double factor_groomed = 0.26;
//
//       genJet = l2l3jet.genJet();
//       smeared = false;
//       if(genJet){
//         double deltaPt = fabs(genJet->pt() - l2l3jet.pt());
//         double deltaR = reco::deltaR(genJet->p4(),l2l3jet.p4());	
//        if (deltaR < 0.4 && deltaPt <= 3*l2l3jet.pt()*res){
//         double deltapt_groomed = (l2l3jet.pt() - genJet->pt()) * factor_groomed;
//         ptscale_groomed = std::max(0.0, (l2l3jet.pt() + deltapt_groomed) / l2l3jet.pt());
//   	     smeared = true;
//       }
//     }
//     if (!smeared && factor_groomed>0) {
//       gRandom->SetSeed(abs(static_cast<int>(l2l3jet.phi()*1e4)));
//       ptscale_groomed = std::max(0.0, gRandom->Gaus(l2l3jet.pt(),sqrt(factor_groomed*(factor_groomed+2))*res*l2l3jet.pt())/l2l3jet.pt());
//     }

     
     //////////////// Now ready to construct tags /////////////////////////
     float PN_TvsQCD = (double)corrak8.bDiscriminator("pfParticleNetDiscriminatorsJetTags:TvsQCD");
     float PN_WvsQCD = (double)corrak8.bDiscriminator("pfParticleNetDiscriminatorsJetTags:WvsQCD");

     int PNW = 0;
     int PNT = 0;

     //W tagging values from 2017 twiki
     if(corrak8.pt() >= 200 && PN_TvsQCD > 0.581 ){
       PNT = 1;
     }
     if(corrak8.pt() >= 200 && PN_WvsQCD > 0.944){
       PNW = 1;
     }
      
     PNWtag.push_back(PNW);
     PNTtag.push_back(PNT);

     int isSubSDcsv1bTag = 0;
     int isSubSDcsv2bTag = 0;
     int Nsub = 0;
     int NCSVM = 0;

     //subjets
     auto const & sdSubjets = corrak8.subjets("SoftDropPuppi");
     for ( auto const & it : sdSubjets ) {
       Nsub++;
       pat::Jet corrsubjet = *it;
       double SDsubjetBdisc = corrsubjet.bDiscriminator("pfDeepCSVJetTags:probb") + corrsubjet.bDiscriminator("pfDeepCSVJetTags:probbb");
       if(SDsubjetBdisc > discriminatorValue) NCSVM++;

     }

     if(Nsub > 1  && NCSVM == 1 && 120 < corrak8.pt() && corrak8.pt() < 180) isSubSDcsv1bTag = 1;
     if(Nsub > 1  && NCSVM == 2 && 120 < corrak8.pt() && corrak8.pt() < 180) isSubSDcsv2bTag = 1;

     SubSDcsv1bTag.push_back(isSubSDcsv1bTag);
     SubSDcsv2bTag.push_back(isSubSDcsv2bTag);


   }
     std::cout << "Found " << jetPt.size() << " fat jets in this event" << std::endl;

   ///////////// Get the generated particle collection ////////////////
   edm::Handle<reco::GenParticleCollection> genParticles;
   if(iEvent.getByToken(GPtoken, genParticles)){
    
     // loop over all gen particles in event
     for(size_t i = 0; i < genParticles->size(); i++){
       const reco::GenParticle &p = (*genParticles).at(i);
       int id = p.pdgId();
      
       bool hasRadiation = false;
       bool hasLepton = false;
      
       //////////////// First identify bosons and see if they are hadronic ///////////////////

       if(abs(id) == 23 || abs(id) == 24 || abs(id) == 25 || abs(id) == 6){
	
   	     size_t nDs = p.numberOfDaughters();
   	     for(size_t j = 0; j < nDs; j++){
   	      int dauId = (p.daughter(j))->pdgId();
   	      const reco::Candidate *d = p.daughter(j);
   	      if(d->pdgId() != dauId) std::cout << "making daughter GenParticle didn't work" << std::endl;
	  
   	      if(abs(dauId) == abs(id)) hasRadiation = true;
   	      else if(abs(dauId) == 24){  // check t->Wb->leptons and H->WW->leptons
   	        while(d->numberOfDaughters() == 1) d = d->daughter(0);
   	        if(abs(d->daughter(0)->pdgId()) > 10 && abs(d->daughter(0)->pdgId()) < 17) hasLepton = true;
   	        if(abs(d->daughter(1)->pdgId()) > 10 && abs(d->daughter(1)->pdgId()) < 17) hasLepton = true;
          }   
   	      else if(abs(dauId) == 23){  // check H->ZZ->leptons
   	        while(d->numberOfDaughters() == 1) d = d->daughter(0);
   	        if(abs(d->daughter(0)->pdgId()) > 10 && abs(d->daughter(0)->pdgId()) < 17) hasLepton = true;
   	        if(abs(d->daughter(1)->pdgId()) > 10 && abs(d->daughter(1)->pdgId()) < 17) hasLepton = true;
   	      }
   	      else if(abs(dauId) > 10 && abs(dauId) < 17) hasLepton = true;	    
	  
   	     }
	
   	if(hasRadiation) continue;	  
   	if(hasLepton) continue;	  
   	//if(p.pt() < 175) continue;
	
   	////////////// Check for W's from H -> WW, or t->Wb /////////////////////
   	if(abs(id) == 24){
   	  double dRWb = 1000;
   	  double dRWW = 1000;
	  
   	  const reco::Candidate *mother = p.mother();
   	  while(abs(mother->pdgId()) == 24) mother = mother->mother();
	  
   	  if(abs(mother->pdgId()) == 6){
   	    double dr = reco::deltaR(p.p4(),mother->daughter(1)->p4());
   	    if(abs(mother->daughter(1)->pdgId()) == 24) dr = reco::deltaR(p.p4(),mother->daughter(0)->p4());
   	    if(dr < dRWb) dRWb = dr;
   	  }else if(abs(mother->pdgId()) == 25){
   	    double dr = 1000;
   	    if(p.pdgId()*mother->daughter(0)->pdgId() > 0){
   	      dr = reco::deltaR(p.p4(),mother->daughter(1)->p4());
   	    }else{
   	      dr = reco::deltaR(p.p4(),mother->daughter(0)->p4());
   	    }
   	    if(dr < dRWW) dRWW = dr;
   	  }		
	  
   	  if(dRWW < 0.8) continue; // W from merged H
   	  if(dRWb < 0.8) continue; // W from merged t
   	}
	
   	/////////////// Check for Z's from H -> ZZ /////////////////
   	if(abs(id) == 23){
   	  double dRZZ = 1000;
	  
   	  const reco::Candidate *mother = p.mother();
   	  while(abs(mother->pdgId()) == 23) mother = mother->mother();
	  
   	  if(abs(mother->pdgId()) == 25){
   	    double dr = 1000;
   	    if(p.pdgId()*mother->daughter(0)->pdgId() > 0){
   	      dr = reco::deltaR(p.p4(),mother->daughter(1)->p4());
   	    }else{
   	      dr = reco::deltaR(p.p4(),mother->daughter(0)->p4());
   	    }
   	    if(dr < dRZZ) dRZZ = dr;
   	  }
	  
   	  if(dRZZ < 0.8) continue; // Z from merged H
   	}
	
   	//////////// Check that nothing has less than 2 daughters //////////////////
   	if(p.numberOfDaughters() < 2){
   	  std::cout << p.numberOfDaughters() << " daughters from " << p.pdgId() << std::endl;
   	  continue;
   	}
   	if(abs(id) == 23) continue; 

   	///////////// Now check W's for tag matches /////////////////////
   	if(abs(id) == 24){
   	  TLorentzVector d1,d2,tempW,trueW,jetW;
   	  d1.SetPtEtaPhiE(p.daughter(0)->pt(),p.daughter(0)->eta(),p.daughter(0)->phi(),p.daughter(0)->energy());
   	  d2.SetPtEtaPhiE(p.daughter(1)->pt(),p.daughter(1)->eta(),p.daughter(1)->phi(),p.daughter(1)->energy());
   	  trueW.SetPtEtaPhiE(p.pt(),p.eta(),p.phi(),p.energy());
   	  tempW.SetPtEtaPhiE(0,0,0,0);
   	  jetW.SetPtEtaPhiE(0,0,0,0);
	  
   	  double minDR = 1000;
   	  int matchtag = -1;
   	  for(unsigned int itag = 0; itag < jetPt.size(); itag++){
   	    tempW.SetPtEtaPhiE(jetPt.at(itag),jetEta.at(itag),jetPhi.at(itag),jetEnergy.at(itag));
   	    double tempDR = trueW.DeltaR(tempW);
   	    if(tempDR < minDR){
   	      minDR = tempDR;
   	      matchtag = itag;
   	      jetW = tempW;
   	    }
   	  }
   	  if(minDR > 0.8) continue;
	  
   	  bool WallDsInJet = false;
   	  if(jetW.DeltaR(d1) < 0.8 && jetW.DeltaR(d2) < 0.8) WallDsInJet = true;
   	  if(!WallDsInJet) continue;

   	  double ptfiller = p.pt();
   	  if(p.pt() > 2000) ptfiller = 1999;
	  
   	  // Fill denominator with true Ws matched to AK8 jets
   	  WEff_Dpteta->Fill(ptfiller,p.eta(),weight);
   	  WEff_DptbinsMatch->Fill(ptfiller,weight);

   	  // Fill denominator with true Ws matched to AK8 jets
   	  // Fill numerator if the matched jet passes PN W requirments
   	  WEff_DptbinsMass->Fill(ptfiller,weight);
   	  if(PNWtag.at(matchtag) == 1){ WEff_NptbinsPN->Fill(ptfiller,weight);}

      }

   	//////////// Now check Higgs for tag matches ///////////////
   	if(abs(id) == 25){

   	  if(abs(p.daughter(0)->pdgId()) != 5 || abs(p.daughter(1)->pdgId()) != 5) continue; // not H -> bb
	
   	  TLorentzVector d1,d2,tempH,trueH,jetH;	  
   	  d1.SetPtEtaPhiE(p.daughter(0)->pt(),p.daughter(0)->eta(),p.daughter(0)->phi(),p.daughter(0)->energy());
   	  d2.SetPtEtaPhiE(p.daughter(1)->pt(),p.daughter(1)->eta(),p.daughter(1)->phi(),p.daughter(1)->energy());
   	  trueH.SetPtEtaPhiE(p.pt(),p.eta(),p.phi(),p.energy());
   	  tempH.SetPtEtaPhiE(0,0,0,0);
   	  jetH.SetPtEtaPhiE(0,0,0,0);
	
   	  double minDR = 1000;
   	  int matchtag = -1;
   	  for(unsigned int itag = 0; itag < jetPt.size(); itag++){
   	    tempH.SetPtEtaPhiE(jetPt.at(itag),jetEta.at(itag),jetPhi.at(itag),jetEnergy.at(itag));
   	    double tempDR = trueH.DeltaR(tempH);
   	    if(tempDR < minDR){
   	      minDR = tempDR;
   	      matchtag = itag;
   	      jetH = tempH;
   	    }
   	  }
   	  if(minDR > 0.8) continue;
	  
   	  bool HallDsInJet = false;
   	  if(jetH.DeltaR(d1) < 0.8 && jetH.DeltaR(d2) < 0.8) HallDsInJet = true;
   	  if(!HallDsInJet) continue;
	  
   	  double ptfiller = p.pt();
   	  if(p.pt() > 2000) ptfiller = 1999;
   	  // Fill denominator with true Ws matched to AK8 jets
   	  HEff1b_Dptbins->Fill(ptfiller,weight);
   	  HEff2b_Dptbins->Fill(ptfiller,weight);
   	  HEff_D->Fill(ptfiller,weight);
	  
   	  // Fill numerator with true Ws matched to tagged jets
   	  if(SubSDcsv1bTag.at(matchtag)){
   	    HEff1b_Nptbins->Fill(ptfiller,weight);
   	    HEff_N->Fill(ptfiller,weight);
   	  }
   	  if(SubSDcsv2bTag.at(matchtag)){
   	    HEff2b_Nptbins->Fill(ptfiller,weight);
   	    HEff_N->Fill(ptfiller,weight);
   	  }	  
   	}

   	////////////////// Now check top for tag matches ///////////////////////
   	if(abs(id) == 6){
   	  TLorentzVector d1,d2,d3,tempT,trueT,jetT;
   	  const reco::Candidate *W = p.daughter(0);
   	  const reco::Candidate *b = p.daughter(1);
   	  if(abs(W->pdgId()) == 5){
   	    W = p.daughter(1);
   	    b = p.daughter(0);
   	  }

   	  while(W->numberOfDaughters() == 1) W = W->daughter(0);	  
   	  d1.SetPtEtaPhiE(W->daughter(0)->pt(),W->daughter(0)->eta(),W->daughter(0)->phi(),W->daughter(0)->energy());
   	  d2.SetPtEtaPhiE(W->daughter(1)->pt(),W->daughter(1)->eta(),W->daughter(1)->phi(),W->daughter(1)->energy());
   	  d3.SetPtEtaPhiE(b->pt(),b->eta(),b->phi(),b->energy());	  
   	  trueT.SetPtEtaPhiE(p.pt(),p.eta(),p.phi(),p.energy());
   	  tempT.SetPtEtaPhiE(0,0,0,0);
   	  jetT.SetPtEtaPhiE(0,0,0,0);

   	  double minDR = 1000;
   	  int matchtag = -1;
   	  for(unsigned int itag = 0; itag < jetPt.size(); itag++){
   	    tempT.SetPtEtaPhiE(jetPt.at(itag),jetEta.at(itag),jetPhi.at(itag),jetEnergy.at(itag));
   	    double tempDR = trueT.DeltaR(tempT);
   	    if(tempDR < minDR){
  	      minDR = tempDR;
  	      matchtag = itag;
   	      jetT = tempT;
   	    }
   	  }
   	  if(minDR > 0.8) continue;
	  
   	  bool TallDsInJet = false;
   	  if(jetT.DeltaR(d1) < 0.8 && jetT.DeltaR(d2) < 0.8 && jetT.DeltaR(d3) < 0.8) TallDsInJet = true;
   	  if(!TallDsInJet) continue;

   	  double ptfiller = p.pt();
   	  if(p.pt() > 2000) ptfiller = 1999;
	  
   	  // Fill denominator with true tops matched to AK8 jets
   	  TEff_DptbinsMatch->Fill(ptfiller,weight);

   	  // Fill denominator with true tops matched to AK8 jets in the mass window
   	  // Fill numerator if the matched jet is tau32 tagged
   	  TEff_DptbinsMass->Fill(ptfiller,weight);
   	  if(PNTtag.at(matchtag) == 1){ TEff_NptbinsPN->Fill(ptfiller,weight);}
      
      }
      }
    }
}
}

// ------------ method called once each job just before starting event loop  ------------
void 
WeightAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
WeightAnalyzer::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
/*
void 
WeightAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}


// ------------ method called when ending the processing of a run  ------------

void 
WeightAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{
}


// ------------ method called when starting to processes a luminosity block  ------------

void 
WeightAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}


// ------------ method called when ending the processing of a luminosity block  ------------

void 
WeightAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
WeightAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(WeightAnalyzer);
