PATH_TO_PROD="/eos/cms/store/user/lcadamur/DelphesPhaseIITrees"
ls -1 -v ${PATH_TO_PROD}/ttbar_inclusive_prodttbar_6Ott_full/p2ntuple_*.root > ttbar_inclusive.txt
source replaceEOSserver.sh ttbar_inclusive.txt