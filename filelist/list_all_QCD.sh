PATH_TO_PROD="/eos/cms/store/user/lcadamur/DelphesPhaseIITrees"

ls -1 -v ${PATH_TO_PROD}/QCD_bEnriched_HT200to300_prodQCD_17Sep_full/p2ntuple_*.root   > QCD_bEnriched_HT200to300.txt
ls -1 -v ${PATH_TO_PROD}/QCD_bEnriched_HT300to500_prodQCD_17Sep_full/p2ntuple_*.root   > QCD_bEnriched_HT300to500.txt
ls -1 -v ${PATH_TO_PROD}/QCD_bEnriched_HT500to700_prodQCD_17Sep_full/p2ntuple_*.root   > QCD_bEnriched_HT500to700.txt
ls -1 -v ${PATH_TO_PROD}/QCD_bEnriched_HT700to1000_prodQCD_17Sep_full/p2ntuple_*.root  > QCD_bEnriched_HT700to1000.txt
ls -1 -v ${PATH_TO_PROD}/QCD_bEnriched_HT1000to1500_prodQCD_17Sep_full/p2ntuple_*.root > QCD_bEnriched_HT1000to1500.txt
ls -1 -v ${PATH_TO_PROD}/QCD_bEnriched_HT1500to2000_prodQCD_17Sep_full/p2ntuple_*.root > QCD_bEnriched_HT1500to2000.txt
ls -1 -v ${PATH_TO_PROD}/QCD_bEnriched_HT2000toInf_prodQCD_17Sep_full/p2ntuple_*.root  > QCD_bEnriched_HT2000toInf.txt

source replaceEOSserver.sh QCD_bEnriched_HT200to300.txt
source replaceEOSserver.sh QCD_bEnriched_HT300to500.txt
source replaceEOSserver.sh QCD_bEnriched_HT500to700.txt
source replaceEOSserver.sh QCD_bEnriched_HT700to1000.txt
source replaceEOSserver.sh QCD_bEnriched_HT1000to1500.txt
source replaceEOSserver.sh QCD_bEnriched_HT1500to2000.txt
source replaceEOSserver.sh QCD_bEnriched_HT2000toInf.txt