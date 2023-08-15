import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 5000

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32 ( -1 ) )

process.source = cms.Source( "PoolSource",
#### FOR TESTING
#                             fileNames = cms.untracked.vstring(
#'root://cmsxrootd.fnal.gov///store/mc/RunIISummer20UL17MiniAODv2/PairVLQ_x53x53_tWtW_narrow_RH_M1000_TuneCP5_13TeV-madgraph-pythia8/MINIAODSIM/106X_mc2017_realistic_v9-v2/70000/A5508485-985D-0B4E-AF6C-F0D83704EE13.root'),
#  'root://cmsxrootd.fnal.gov//store/mc/RunIIFall17MiniAODv2/TTTT_TuneCP5_13TeV-amcatnlo-pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/70000/E2849E6E-B45A-E911-BCFB-484D7E8DF09F.root'),
# 'root://cmsxrootd.fnal.gov//store/mc/RunIIFall17MiniAODv2/X53X53_M-1600_RH_TuneCP5_13TeV-madgraph-pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/270000/2A41980C-BC5C-E911-A69D-7845C4FC3A7F.root'),
#        'root://cmsxrootd.fnal.gov//store/mc/RunIIFall17MiniAODv2/TprimeTprime_M-1000_TuneCP5_13TeV-madgraph-pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14-v3/100000/8ABEDA01-2BAC-E811-A04C-24BE05CECBE1.root'),
#'root://cmsxrootd.fnal.gov//store/mc/RunIIFall17MiniAODv2/ST_t-channel_top_4f_InclusiveDecays_TuneCP5_PSweights_13TeV-powheg-pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/100000/1273B66C-2683-E811-A047-0090FAA57350.root'),
#'/store/mc/RunIIAutumn18MiniAOD/TprimeTprime_M-1000_TuneCP5_PSweights_13TeV-madgraph-pythia8/MINIAODSIM/102X_upgrade2018_realistic_v15-v2/30000/C5EF9915-7DF1-414C-89B0-2FB772A33FDC.root'),
# 'root://cmsxrootd.fnal.gov//store/mc/RunIIFall17MiniAODv2/X53X53_M-1600_RH_TuneCP5_13TeV-madgraph-pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/270000/7402E27A-0B5E-E911-8AB7-A0369F30FFD2.root'),
#'root://cmsxrootd.fnal.gov//store/mc/RunIIAutumn18MiniAOD/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/MINIAODSIM/102X_upgrade2018_realistic_v15-v1/80000/F8A36AF6-E341-4741-A39F-3A277A3B77AC.root'),
#'root://cmsxrootd.fnal.gov//store/mc/RunIIAutumn18MiniAOD/X53X53_M-1000_RH_TuneCP5_13TeV-madgraph-pythia8/MINIAODSIM/102X_upgrade2018_realistic_v15-v1/260000/9C300E11-BD11-9341-B84D-7CFDC25F6A90.root'),
#### FOR CONDOR
                           fileNames = cms.untracked.vstring(CONDOR_FILELIST),
                           inputCommands = cms.untracked.vstring('keep *'),
                             )

#DiscriminatorValue = 0.4506 (2017), 0.4506 (2018)
#run the producer -- USE WHICHEVER CONFIG ARGUMENTS NEEDED
process.WeightAnalyzer = cms.EDAnalyzer(
      "WeightAnalyzer",
       #LHEEPTag   = cms.InputTag('externalLHEProducer'),
       #isX53 = cms.bool(True),
       #isWJ = cms.bool(True),
       JetsTag            = cms.InputTag('slimmedJets'),
       JetsTagAK8         = cms.InputTag('slimmedJetsAK8'),
       genParticles_it    = cms.InputTag('prunedGenParticles'),
       DiscriminatorTag   = cms.string('pfDeepFlavourJetTags'),
       DiscriminatorValue = cms.double(0.89104), #0.89104 2017 0.90373 2018 https://gitlab.cern.ch/cms-btv/btv-json-sf/-/blob/master/data/UL2017/subjet_deepCSV.csv?plain=1
       PtNBins            = cms.int32(50),
       PtMin              = cms.double(0.),
       PtMax              = cms.double(1000.),
       EtaNBins           = cms.int32(30),
       EtaMin             = cms.double(-3.0),
       EtaMax             = cms.double(3.0)
    )

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string('CONDOR_OUTFILE.root') # for condor
#                                   fileName = cms.string('test2.root') # for testing
                                   )

process.p = cms.Path(process.WeightAnalyzer)




