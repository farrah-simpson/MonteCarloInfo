import os,sys

#### Possible options
#### --sample   Type of sample (e.g. TTbar)
#### --fileList File with a list of root files to be processed
#### --outDir   Directory to put output of script
#### --submit   Whether to submit the jobs to condor (True or False)

samplelist = [

#    'X53700.txt',
#    'X53800.txt',
#    'X53900.txt',
#    'X531000.txt',
#    'X531100.txt',
#    'X531200.txt',
#    'X531300.txt',
#    'X531400.txt',
#    'X531500.txt',
#    'X531600.txt',
#    'VVout.txt',
#    'STout.txt',
    'TTout.txt',
#    'STtWout.txt', 
#    'STtout.txt', 
#    'WVout.txt',

   # 'X53tHMX1200MH1000.txt',
   # 'X53tHMX1200MH800.txt',
   # 'X53tHMX1200MH600.txt',
   # 'X53tHMX1200MH400.txt',
   # 'X53tHMX1200MH200.txt'
   # 'X53X53_M-900_RH_TuneCP5_13TeV-madgraph-pythia8.txt',
   # 'X53X53_M-1000_RH_TuneCP5_13TeV-madgraph-pythia8.txt',
   # 'X53X53_M-1100_RH_TuneCP5_13TeV-madgraph-pythia8.txt',
   # 'X53X53_M-1100_LH_TuneCP5_13TeV-madgraph-pythia8.txt',
   # 'X53X53_M-1200_LH_TuneCP5_13TeV-madgraph-pythia8.txt',
   # 'X53X53_M-1200_RH_TuneCP5_13TeV-madgraph-pythia8.txt',
   # 'X53X53_M-1300_RH_TuneCP5_13TeV-madgraph-pythia8.txt',
   # 'X53X53_M-1400_RH_TuneCP5_13TeV-madgraph-pythia8.txt',
   # 'X53X53_M-1400_LH_TuneCP5_13TeV-madgraph-pythia8.txt',
   # 'X53X53_M-1500_LH_TuneCP5_13TeV-madgraph-pythia8.txt',
   # 'X53X53_M-1500_RH_TuneCP5_13TeV-madgraph-pythia8.txt',
   # 'X53X53_M-1600_RH_TuneCP5_13TeV-madgraph-pythia8.txt',
   # 'X53X53_M-1700_RH_TuneCP5_13TeV-madgraph-pythia8.txt',
   # 'X53X53_M-1700_LH_TuneCP5_13TeV-madgraph-pythia8.txt',

  #   'BprimeBprime_M-1000_TuneCP5_13TeV-madgraph-pythia8.txt',
  #   'BprimeBprime_M-1100_TuneCP5_13TeV-madgraph-pythia8.txt',
  #   'BprimeBprime_M-1200_TuneCP5_13TeV-madgraph-pythia8.txt',
  #   'BprimeBprime_M-1300_TuneCP5_13TeV-madgraph-pythia8.txt',
  #   'BprimeBprime_M-1400_TuneCP5_13TeV-madgraph-pythia8.txt',
  #   'BprimeBprime_M-1500_TuneCP5_13TeV-madgraph-pythia8.txt',
  #   'BprimeBprime_M-1600_TuneCP5_13TeV-madgraph-pythia8.txt',
  #   'BprimeBprime_M-1700_TuneCP5_13TeV-madgraph-pythia8.txt',     
  #   'BprimeBprime_M-1800_TuneCP5_13TeV-madgraph-pythia8.txt',
  #   'BprimeBprime_M-900_TuneCP5_13TeV-madgraph-pythia8.txt',

  #   'TprimeTprime_M-1000_TuneCP5_13TeV-madgraph-pythia8.txt',
  #   'TprimeTprime_M-700_TuneCP5_13TeV-madgraph-pythia8.txt',
  #   'TprimeTprime_M-1100_TuneCP5_13TeV-madgraph-pythia8.txt',
  #   'TprimeTprime_M-1200_TuneCP5_13TeV-madgraph-pythia8.txt',
  #   'TprimeTprime_M-1300_TuneCP5_13TeV-madgraph-pythia8.txt',
  #   'TprimeTprime_M-1400_TuneCP5_13TeV-madgraph-pythia8.txt',
  #   'TprimeTprime_M-1500_TuneCP5_13TeV-madgraph-pythia8.txt',
  #   'TprimeTprime_M-1600_TuneCP5_13TeV-madgraph-pythia8.txt',
  #   'TprimeTprime_M-1700_TuneCP5_13TeV-madgraph-pythia8.txt',
  #   'TprimeTprime_M-1800_TuneCP5_13TeV-madgraph-pythia8.txt',
  #   'TprimeTprime_M-900_TuneCP5_13TeV-madgraph-pythia8.txt',

  #    'TTTT_TuneCP5_13TeV-amcatnlo-pythia8.txt',

#2017 background
#    'DYJetsToLL_M-50_HT-200to400_TuneCP5_13TeV-madgraphMLM-pythia8.txt',  
#    'DYJetsToLL_M-50_HT-400to600_TuneCP5_13TeV-madgraphMLM-pythia8.txt', 
#    'DYJetsToLL_M-50_HT-600to800_TuneCP5_13TeV-madgraphMLM-pythia8.txt', 
#    'DYJetsToLL_M-50_HT-800to1200_TuneCP5_13TeV-madgraphMLM-pythia8.txt',
#    'DYJetsToLL_M-50_HT-1200to2500_TuneCP5_13TeV-madgraphMLM-pythia8.txt',
#    'DYJetsToLL_M-50_HT-2500toInf_TuneCP5_13TeV-madgraphMLM-pythia8.txt',
#
#     'ttHTobb_M125_TuneCP5_13TeV-powheg-pythia8.txt',
#     'ttHToNonbb_M125_TuneCP5_13TeV-powheg-pythia8.txt',
#     'TTWJetsToQQ_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8.txt',
#     'ttWJets_TuneCP5_13TeV_madgraphMLM_pythia8.txt',
#     'ttZJets_TuneCP5_13TeV_madgraphMLM_pythia8.txt',
#     'TTZToLLNuNu_M-10_TuneCP5_PSweights_13TeV-amcatnlo-pythia8.txt',
#     'TTZToQQ_TuneCP5_13TeV-amcatnlo-pythia8.txt',
#     'TTWJetsToLNu_TuneCP5_PSweights_13TeV-amcatnloFXFX-madspin-pythia8.txt',
#     'TTZToLL_M-1to10_TuneCP5_13TeV-amcatnlo-pythia8.txt',
#     'QCD_HT1000to1500_TuneCP5_13TeV-madgraph-pythia8.txt',
#     'QCD_HT1500to2000_TuneCP5_13TeV-madgraph-pythia8.txt',
#     'QCD_HT2000toInf_TuneCP5_13TeV-madgraph-pythia8.txt',
#     'QCD_HT300to500_TuneCP5_13TeV-madgraph-pythia8.txt',
#     'QCD_HT500to700_TuneCP5_13TeV-madgraph-pythia8.txt',
#     'QCD_HT700to1000_TuneCP5_13TeV-madgraph-pythia8.txt',
#     'ST_t-channel_antitop_5f_TuneCP5_PSweights_13TeV-powheg-madspin-pythia8_vtd_vts_prod.txt',
#     'ST_t-channel_top_5f_TuneCP5_PSweights_13TeV-powheg-madspin-pythia8_vtd_vts_prod.txt',
#     'TTToHadronic_TuneCP5_PSweights_13TeV-powheg-pythia8.txt',
#    'TTTT_TuneCP5_13TeV-amcatnlo-pythia8.txt'
#     'TTHH_TuneCP5_13TeV-madgraph-pythia8.txt',
#     'TTTJ_TuneCP5_13TeV-madgraph-pythia8.txt',
#     'TTTT_TuneCP5_13TeV-amcatnlo-pythia8.txt ',
#     'TTTW_TuneCP5_13TeV-madgraph-pythia8.txt',
#     'TTToSemiLepton_HT500Njet9_TuneCP5_PSweights_13TeV-powheg-pythia8.txt',
#     'TTToSemiLeptonic_TuneCP5down_PSweights_13TeV-powheg-pythia8.txt',
#     'TTToSemiLeptonic_TuneCP5up_PSweights_13TeV-powheg-pythia8.txt',
#     'TTWH_TuneCP5_13TeV-madgraph-pythia8.txt',
#     'TTWW_TuneCP5_13TeV-madgraph-pythia8.txt',
#     'TTWZ_TuneCP5_13TeV-madgraph-pythia8.txt',
#     'TTZH_TuneCP5_13TeV-madgraph-pythia8.txt',
#     'TTZZ_TuneCP5_13TeV-madgraph-pythia8.txt',
#    'TTToSemiLeptonic_TuneCP5up_PSweights_13TeV-powheg-pythia8.txt',
#    'TTToSemiLeptonic_TuneCP5down_PSweights_13TeV-powheg-pythia8.txt',
#    'TTToSemiLepton_HT500Njet9_TuneCP5_PSweights_13TeV-powheg-pythia8.txt',
#    'TTTo2L2Nu_TuneCP5down_PSweights_13TeV-powheg-pythia8.txt',          
#    'TTTo2L2Nu_TuneCP5up_PSweights_13TeV-powheg-pythia8.txt',            
#
#    'ST_tW_antitop_5f_inclusiveDecays_TuneCP5_PSweights_13TeV-powheg-pythia8.txt',
#    'ST_tW_top_5f_inclusiveDecays_TuneCP5_PSweights_13TeV-powheg-pythia8.txt',
#    'ST_t-channel_antitop_4f_InclusiveDecays_TuneCP5_PSweights_13TeV-powheg-pythia8.txt',
#    'ST_t-channel_top_4f_InclusiveDecays_TuneCP5_PSweights_13TeV-powheg-pythia8.txt',
#
#    'ST_s-channel_top_leptonDecays_13TeV-PSweights_powheg-pythia.txt',
#    'ST_s-channel_antitop_leptonDecays_13TeV-PSweights_powheg-pythia.txt',
#
#    'ST_s-channel_4f_leptonDecays_TuneCP5_13TeV-madgraph-pythia8.txt',
#
#    'WW_TuneCP5_13TeV-pythia8.txt',
#    'WZ_TuneCP5_13TeV-pythia8.txt',
#    'ZZ_TuneCP5_13TeV-pythia8.txt',
#
#    'TTToHadronic_TuneCP5_PSweights_13TeV-powheg-pythia8.txt',
#    'TTToSemiLeptonic_TuneCP5_PSweights_13TeV-powheg-pythia8.txt',
#    'TTTo2L2Nu_TuneCP5_PSweights_13TeV-powheg-pythia8.txt',
#    'TT_Mtt-700to1000_TuneCP5_PSweights_13TeV-powheg-pythia8.txt',
#    'TT_Mtt-1000toInf_TuneCP5_PSweights_13TeV-powheg-pythia8.txt',

#    'WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8.txt',
#    'WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8.txt',
#    'WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8.txt',
#    'WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8.txt',
#    'WJetsToLNu_HT-1200To2500_TuneCP5_13TeV-madgraphMLM-pythia8.txt',
#    'WJetsToLNu_HT-2500ToInf_TuneCP5_13TeV-madgraphMLM-pythia8.txt',

#2018

#'ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8.txt',
#'ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8.txt',
#'ST_t-channel_antitop_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8.txt',
#'ST_t-channel_top_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8.txt',
#'ST_s-channel_4f_leptonDecays_TuneCP5_13TeV-madgraph-pythia8.txt',
#
#'QCD_HT700to1000_TuneCP5_13TeV-madgraphMLM-pythia8.txt',
#'QCD_HT500to700_TuneCP5_13TeV-madgraphMLM-pythia8.txt',
#'QCD_HT300to500_TuneCP5_13TeV-madgraphMLM-pythia8.txt',
#'QCD_HT200to300_TuneCP5_13TeV-madgraphMLM-pythia8.txt',
#'QCD_HT2000toInf_TuneCP5_13TeV-madgraphMLM-pythia8.txt',
#'QCD_HT1500to2000_TuneCP5_13TeV-madgraphMLM-pythia8.txt',
#'QCD_HT1000to1500_TuneCP5_13TeV-madgraphMLM-pythia8.txt',

#'W1JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8.txt',
#'W2JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8.txt',
#'W3JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8.txt',
#'W4JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8.txt',

#'DY1JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8.txt',
#'DY2JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8.txt',
#'DY3JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8.txt',
#'DY4JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8.txt',
#
#'WW_TuneCP5_PSweights_13TeV-pythia8.txt',
#'WZ_TuneCP5_PSweights_13TeV-pythia8.txt',
#'ZZ_TuneCP5_13TeV-pythia8.txt',
#
#'TT_Mtt-1000toInf_TuneCP5_13TeV-powheg-pythia8.txt',
#'TTToHadronic_TuneCP5_13TeV-powheg-pythia8.txt',
#'TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8.txt',
#'TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8.txt',
#'TT_Mtt-700to1000_TuneCP5_PSweights_13TeV-powheg-pythia8.txt',
]

for sample in samplelist:
    os.system('python condor_submit.py --sample '+sample.split('.')[0]+' --fileList /home/fsimpson/fileList/'+sample+'  --submit True --outDir /home/fsimpson/TagEffs2017ULPNnew/')

