PROD_TAG="17Ott2018_fullProd_BDTallbkg"
PATH_TO_FLIST="../plotters/filelists"

# python getTaskStatus.py --dir jobs_QCD_bEnriched_HT2000toInf_${PROD_TAG}   --check --flist ${PATH_TO_FLIST}/bbbbTree_QCD_bEnriched_HT2000toInf.txt
# python getTaskStatus.py --dir jobs_QCD_bEnriched_HT200to300_${PROD_TAG}    --check --flist ${PATH_TO_FLIST}/bbbbTree_QCD_bEnriched_HT200to300.txt
# python getTaskStatus.py --dir jobs_QCD_bEnriched_HT300to500_${PROD_TAG}    --check --flist ${PATH_TO_FLIST}/bbbbTree_QCD_bEnriched_HT300to500.txt
# python getTaskStatus.py --dir jobs_QCD_bEnriched_HT500to700_${PROD_TAG}    --check --flist ${PATH_TO_FLIST}/bbbbTree_QCD_bEnriched_HT500to700.txt
# python getTaskStatus.py --dir jobs_QCD_bEnriched_HT700to1000_${PROD_TAG}   --check --flist ${PATH_TO_FLIST}/bbbbTree_QCD_bEnriched_HT700to1000.txt
# python getTaskStatus.py --dir jobs_QCD_bEnriched_HT1000to1500_${PROD_TAG}  --check --flist ${PATH_TO_FLIST}/bbbbTree_QCD_bEnriched_HT1000to1500.txt
# python getTaskStatus.py --dir jobs_QCD_bEnriched_HT1500to2000_${PROD_TAG}  --check --flist ${PATH_TO_FLIST}/bbbbTree_QCD_bEnriched_HT1500to2000.txt
# #
# python getTaskStatus.py --dir jobs_ttbar_inclusive_${PROD_TAG}  --check --flist ${PATH_TO_FLIST}/ttbar_inclusive.txt
# #
####### python getTaskStatus.py --dir jobs_gg_HH_bb_SM_${PROD_TAG}  --check --flist ${PATH_TO_FLIST}/gg_HH_bbbb.txt
# #
# python getTaskStatus.py --dir jobs_ggZH_HToBB_ZToQQ_${PROD_TAG}    --check --flist ${PATH_TO_FLIST}/ggZH_HToBB_ZToQQ.txt
# python getTaskStatus.py --dir jobs_ZH_HToBB_ZToQQ_${PROD_TAG}      --check --flist ${PATH_TO_FLIST}/ZH_HToBB_ZToQQ.txt
# python getTaskStatus.py --dir jobs_WminusH_HToBB_WToQQ_${PROD_TAG} --check --flist ${PATH_TO_FLIST}/WminusH_HToBB_WToQQ.txt
# python getTaskStatus.py --dir jobs_WplusH_HToBB_WToQQ_${PROD_TAG}  --check --flist ${PATH_TO_FLIST}/WplusH_HToBB_WToQQ.txt
# python getTaskStatus.py --dir jobs_GluGluHToBB_${PROD_TAG}         --check --flist ${PATH_TO_FLIST}/GluGluHToBB.txt
# python getTaskStatus.py --dir jobs_VBFHToBB_${PROD_TAG}            --check --flist ${PATH_TO_FLIST}/VBFHToBB.txt
# python getTaskStatus.py --dir jobs_ttHTobb_${PROD_TAG}             --check --flist ${PATH_TO_FLIST}/ttHTobb.txt

###################

# python hadd_prod.py --flist ${PATH_TO_FLIST}/bbbbTree_QCD_bEnriched_HT2000toInf.txt
# python hadd_prod.py --flist ${PATH_TO_FLIST}/bbbbTree_QCD_bEnriched_HT200to300.txt
# python hadd_prod.py --flist ${PATH_TO_FLIST}/bbbbTree_QCD_bEnriched_HT300to500.txt
# python hadd_prod.py --flist ${PATH_TO_FLIST}/bbbbTree_QCD_bEnriched_HT500to700.txt
# python hadd_prod.py --flist ${PATH_TO_FLIST}/bbbbTree_QCD_bEnriched_HT700to1000.txt
# python hadd_prod.py --flist ${PATH_TO_FLIST}/bbbbTree_QCD_bEnriched_HT1000to1500.txt
# python hadd_prod.py --flist ${PATH_TO_FLIST}/bbbbTree_QCD_bEnriched_HT1500to2000.txt
# #
# python hadd_prod.py --flist ${PATH_TO_FLIST}/ttbar_inclusive.txt
# #
# python hadd_prod.py --flist ${PATH_TO_FLIST}/gg_HH_bbbb.txt

##################

for i in `seq -f %g -5 0.25 10`; do
    python getTaskStatus.py --dir jobs_gg_HH_bb_kl_${i}_${PROD_TAG} --check --flist ${PATH_TO_FLIST}/gg_HH_bb_kl_${i}.txt
done

