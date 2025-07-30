#!/bin/bash
#/cmsuf/data/store/user/nrawal/CSC_NTuples_SingleIsoMuonTrigger/SingleMuon_2017/SingleMuon/crab_SingleMuon_Run2017B-ZMu-09Aug2019_UL2017-v1
#/cmsuf/data/store/user/nrawal/CSC_NTuples_SingleIsoMuonTrigger/SingleMuon_2017/SingleMuon/crab_SingleMuon_Run2017C-ZMu-09Aug2019_UL2017-v1
#/cmsuf/data/store/user/nrawal/CSC_NTuples_SingleIsoMuonTrigger/SingleMuon_2017/SingleMuon/crab_SingleMuon_Run2017D-ZMu-09Aug2019_UL2017-v1
#/cmsuf/data/store/user/nrawal/CSC_NTuples_SingleIsoMuonTrigger/SingleMuon_2017/SingleMuon/crab_SingleMuon_Run2017D-ZMu-09Aug2019_UL2017-v1_missingLumis2
#/cmsuf/data/store/user/nrawal/CSC_NTuples_SingleIsoMuonTrigger/SingleMuon_2017/SingleMuon/crab_SingleMuon_Run2017E-ZMu-09Aug2019_UL2017-v1
#/cmsuf/data/store/user/nrawal/CSC_NTuples_SingleIsoMuonTrigger/SingleMuon_2017/SingleMuon/crab_SingleMuon_Run2017E-ZMu-09Aug2019_UL2017-v1_missingLumis2
#/cmsuf/data/store/user/nrawal/CSC_NTuples_SingleIsoMuonTrigger/SingleMuon_2017/SingleMuon/crab_SingleMuon_Run2017F-ZMu-09Aug2019_UL2017-v1
#/cmsuf/data/store/user/nrawal/CSC_NTuples_SingleIsoMuonTrigger/SingleMuon_2017/SingleMuon/crab_SingleMuon_Run2017F-ZMu-09Aug2019_UL2017-v1_missingLumis

python3 Submitting_Slurmjobs.py  --dataset C --year 2017 --input_path  /cmsuf/data/store/user/nrawal/CSC_NTuples_SingleIsoMuonTrigger/SingleMuon_2017/SingleMuon/crab_SingleMuon_Run2017C-ZMu-09Aug2019_UL2017-v1/ --output_path /cmsuf/data/store/user/t2/users/neha.rawal/CSCAgeing_outputs_Run2/InputFiles_Reprocessed/2017/2017C/ --number 1

#python3 Submitting_Slurmjobs.py  --dataset C --year 2017 --input_path  /cmsuf/data/store/user/nrawal/CSC_NTuples_SingleIsoMuonTrigger/SingleMuon_2017/SingleMuon/crab_SingleMuon_Run2017C-ZMu-09Aug2019_UL2017-v1/ --output_path /cmsuf/data/store/user/t2/users/neha.rawal/CSCAgeing_outputs_Run2/InputFiles_Reprocessed/2017/2017C/ --number 1

#python3 Submitting_Slurmjobs.py  --dataset D --year 2017 --input_path  /cmsuf/data/store/user/nrawal/CSC_NTuples_SingleIsoMuonTrigger/SingleMuon_2017/SingleMuon/crab_SingleMuon_Run2017D-ZMu-09Aug2019_UL2017-v1/ --output_path /cmsuf/data/store/user/t2/users/neha.rawal/CSCAgeing_outputs_Run2/InputFiles_Reprocessed/2017/2017D/ --number 1
python3 Submitting_Slurmjobs.py  --dataset F --year 2017 --input_path  /cmsuf/data/store/user/nrawal/CSC_NTuples_SingleIsoMuonTrigger/SingleMuon_2017/SingleMuon/crab_SingleMuon_Run2017F-ZMu-09Aug2019_UL2017-v1/ --output_path /cmsuf/data/store/user/t2/users/neha.rawal/CSCAgeing_outputs_Run2/InputFiles_Reprocessed/2017/2017F/ --number 1

python3 Submitting_Slurmjobs.py  --dataset F --year 2017 --input_path  /cmsuf/data/store/user/nrawal/CSC_NTuples_SingleIsoMuonTrigger/SingleMuon_2017/SingleMuon/crab_SingleMuon_Run2017F-ZMu-09Aug2019_UL2017-v1_missingLumis/ --output_path /cmsuf/data/store/user/t2/users/neha.rawal/CSCAgeing_outputs_Run2/InputFiles_Reprocessed/2017/2017F/ --number 2
