PATH_TO_PROD="/eos/cms/store/user/lcadamur/DelphesPhaseIITrees"

ls -1 -v ${PATH_TO_PROD}/ggZH_HToBB_ZToQQ_prodSingleH_7Ott_full/p2ntuple_*.root     > ggZH_HToBB_ZToQQ.txt
ls -1 -v ${PATH_TO_PROD}/GluGluHToBB_prodSingleH_7Ott_full/p2ntuple_*.root          > GluGluHToBB.txt
ls -1 -v ${PATH_TO_PROD}/ZH_HToBB_ZToQQ_prodSingleH_7Ott_full/p2ntuple_*.root       > ZH_HToBB_ZToQQ.txt
ls -1 -v ${PATH_TO_PROD}/WplusH_HToBB_WToQQ_prodSingleH_7Ott_full/p2ntuple_*.root   > WplusH_HToBB_WToQQ.txt
ls -1 -v ${PATH_TO_PROD}/WminusH_HToBB_WToQQ_prodSingleH_7Ott_full/p2ntuple_*.root  > WminusH_HToBB_WToQQ.txt
ls -1 -v ${PATH_TO_PROD}/VBFHToBB_prodSingleH_7Ott_full/p2ntuple_*.root             > VBFHToBB.txt
ls -1 -v ${PATH_TO_PROD}/ttHTobb_prodSingleH_7Ott_full/p2ntuple_*.root              > ttHTobb.txt

source replaceEOSserver.sh ggZH_HToBB_ZToQQ.txt
source replaceEOSserver.sh GluGluHToBB.txt
source replaceEOSserver.sh ZH_HToBB_ZToQQ.txt
source replaceEOSserver.sh WplusH_HToBB_WToQQ.txt
source replaceEOSserver.sh WminusH_HToBB_WToQQ.txt
source replaceEOSserver.sh VBFHToBB.txt
source replaceEOSserver.sh ttHTobb.txt
