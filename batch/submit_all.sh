TAG='17Ott2018_fullProd_BDTallbkg'

# python submitOnBatch.py --input '../filelist/QCD_bEnriched_HT200to300.txt'   --tag QCD_bEnriched_HT200to300_${TAG}   --xs 100300 --njobs 25
# python submitOnBatch.py --input '../filelist/QCD_bEnriched_HT300to500.txt'   --tag QCD_bEnriched_HT300to500_${TAG}   --xs 21730  --njobs 25
# python submitOnBatch.py --input '../filelist/QCD_bEnriched_HT500to700.txt'   --tag QCD_bEnriched_HT500to700_${TAG}   --xs 1945   --njobs 25
# python submitOnBatch.py --input '../filelist/QCD_bEnriched_HT700to1000.txt'  --tag QCD_bEnriched_HT700to1000_${TAG}  --xs 380.6  --njobs 25
# python submitOnBatch.py --input '../filelist/QCD_bEnriched_HT1000to1500.txt' --tag QCD_bEnriched_HT1000to1500_${TAG} --xs 68.81  --njobs 25
# python submitOnBatch.py --input '../filelist/QCD_bEnriched_HT1500to2000.txt' --tag QCD_bEnriched_HT1500to2000_${TAG} --xs 6.053  --njobs 25
# python submitOnBatch.py --input '../filelist/QCD_bEnriched_HT2000toInf.txt'  --tag QCD_bEnriched_HT2000toInf_${TAG}  --xs 1.087  --njobs 25
# ##
# python submitOnBatch.py --input '../filelist/ttbar_inclusive.txt'                 --tag ttbar_inclusive_${TAG}  --xs 984.50  --njobs 25
# #####     
# #### xs from here : https://twiki.cern.ch/twiki/bin/view/LHCPhysics/CERNYellowReportPageAt14TeV#ggZH_Cross_Section
# #### mH = 125 GeV
# #### B(H > bb) = 0.5824
# #### B(Z > qq) = 0.6991
# #### B(W > qq) = 0.6760

# #### NOTE: for ZH
# #### sigma(pp -> ZH) = 0.9861
# #### sigma(gg -> ZH) = 0.1443
# #### sigma(gg -> ZH) used for ggZH sample
# #### sigma(pp -> ZH) - sigma(gg -> ZH) used for ZH sample


# python submitOnBatch.py --input '../filelist/ZH_HToBB_ZToQQ.txt'       --tag ZH_HToBB_ZToQQ_${TAG}       --xs 0.343   --njobs 25
# python submitOnBatch.py --input '../filelist/ggZH_HToBB_ZToQQ.txt'     --tag ggZH_HToBB_ZToQQ_${TAG}     --xs 0.05875 --njobs 25
# python submitOnBatch.py --input '../filelist/WplusH_HToBB_WToQQ.txt'   --tag WplusH_HToBB_WToQQ_${TAG}   --xs 0.2978  --njobs 25
# python submitOnBatch.py --input '../filelist/WminusH_HToBB_WToQQ.txt'  --tag WminusH_HToBB_WToQQ_${TAG}  --xs 0.2978  --njobs 25
# python submitOnBatch.py --input '../filelist/GluGluHToBB.txt'          --tag GluGluHToBB_${TAG}          --xs 31.84   --njobs 25
# python submitOnBatch.py --input '../filelist/VBFHToBB.txt'             --tag VBFHToBB_${TAG}             --xs 2.49    --njobs 25
# python submitOnBatch.py --input '../filelist/ttHTobb.txt'              --tag ttHTobb_${TAG}              --xs 0.357   --njobs 25

#### FIXME: bbH -> no delphes sample

##
##### the inclusive one for the SM
# python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'                 --tag gg_HH_bb_SM_${TAG}  --xs 0.01233  --njobs 10
##### reweighted samples

# NOTE: you can loop over for scripts as
# for i in `seq -f %g -5 0.25 10`; do echo $i; done
# the -f %g command will remove trailing zeroes in the decimal expansion
# for i in `seq -f %g -5 0.25 10`; do echo $i; python getTaskStatus.py --dir jobs_gg_HH_bb_kl_${i}_17Ott2018_fullProd_BDTallbkg  | grep Failed ; done
# for i in `seq -f %g -5 0.25 10`; do echo $i; python getTaskStatus.py --dir jobs_gg_HH_bb_kl_${i}_17Ott2018_fullProd_BDTallbkg  | grep Success ; done

python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_-5_${TAG}     --kl -5      --xs 0.200016027    --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_-4.75_${TAG}  --kl -4.75   --xs 0.187104051    --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_-4.5_${TAG}   --kl -4.5    --xs 0.174634722    --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_-4.25_${TAG}  --kl -4.25   --xs 0.16260804     --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_-4_${TAG}     --kl -4      --xs 0.151024005    --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_-3.75_${TAG}  --kl -3.75   --xs 0.13988385     --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_-3.5_${TAG}   --kl -3.5    --xs 0.129185109    --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_-3.25_${TAG}  --kl -3.25   --xs 0.1189296315   --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_-3_${TAG}     --kl -3      --xs 0.109116801    --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_-2.75_${TAG}  --kl -2.75   --xs 0.0997467408   --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_-2.5_${TAG}   --kl -2.5    --xs 0.0908194509   --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_-2.25_${TAG}  --kl -2.25   --xs 0.0823349313   --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_-2_${TAG}     --kl -2      --xs 0.0742930587   --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_-1.75_${TAG}  --kl -1.75   --xs 0.0666939564   --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_-1.5_${TAG}   --kl -1.5    --xs 0.0595376244   --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_-1.25_${TAG}  --kl -1.25   --xs 0.0528239394   --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_-1_${TAG}     --kl -1      --xs 0.0465530247   --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_-0.75_${TAG}  --kl -0.75   --xs 0.0407248803   --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_-0.5_${TAG}   --kl -0.5    --xs 0.0353395062   --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_-0.25_${TAG}  --kl -0.25   --xs 0.0303969024   --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_0_${TAG}      --kl 0       --xs 0.0258969456   --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_0.25_${TAG}   --kl 0.25    --xs 0.0218397591   --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_0.5_${TAG}    --kl 0.5     --xs 0.0182252196   --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_0.75_${TAG}   --kl 0.75    --xs 0.0150535737   --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_1_${TAG}      --kl 1       --xs 0.0123245748   --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_1.25_${TAG}   --kl 1.25    --xs 0.01003833387  --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_1.5_${TAG}    --kl 1.5     --xs 0.00819482625  --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_1.75_${TAG}   --kl 1.75    --xs 0.00679406427  --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_2_${TAG}      --kl 2       --xs 0.0058360356   --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_2.25_${TAG}   --kl 2.25    --xs 0.00532074024  --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_2.5_${TAG}    --kl 2.5     --xs 0.00524819052  --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_2.75_${TAG}   --kl 2.75    --xs 0.00561837411  --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_3_${TAG}      --kl 3       --xs 0.00643130334  --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_3.25_${TAG}   --kl 3.25    --xs 0.00768696588  --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_3.5_${TAG}    --kl 3.5     --xs 0.00938536173  --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_3.75_${TAG}   --kl 3.75    --xs 0.01152650322  --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_4_${TAG}      --kl 4       --xs 0.0141103287   --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_4.25_${TAG}   --kl 4.25    --xs 0.0171369738   --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_4.5_${TAG}    --kl 4.5     --xs 0.0206063892   --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_4.75_${TAG}   --kl 4.75    --xs 0.0245184516   --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_5_${TAG}      --kl 5       --xs 0.0288732843   --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_5.25_${TAG}   --kl 5.25    --xs 0.033670764    --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_5.5_${TAG}    --kl 5.5     --xs 0.0389111373   --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_5.75_${TAG}   --kl 5.75    --xs 0.0445941576   --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_6_${TAG}      --kl 6       --xs 0.0507199482   --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_6.25_${TAG}   --kl 6.25    --xs 0.0572885091   --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_6.5_${TAG}    --kl 6.5     --xs 0.064299717    --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_6.75_${TAG}   --kl 6.75    --xs 0.0717536952   --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_7_${TAG}      --kl 7       --xs 0.0796504437   --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_7.25_${TAG}   --kl 7.25    --xs 0.0879899625   --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_7.5_${TAG}    --kl 7.5     --xs 0.0967721283   --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_7.75_${TAG}   --kl 7.75    --xs 0.1059970644   --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_8_${TAG}      --kl 8       --xs 0.1156647708   --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_8.25_${TAG}   --kl 8.25    --xs 0.125774631    --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_8.5_${TAG}    --kl 8.5     --xs 0.136327878    --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_8.75_${TAG}   --kl 8.75    --xs 0.147323772    --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_9_${TAG}      --kl 9       --xs 0.158762313    --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_9.25_${TAG}   --kl 9.25    --xs 0.170644734    --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_9.5_${TAG}    --kl 9.5     --xs 0.182968569    --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_9.75_${TAG}   --kl 9.75    --xs 0.195735051    --njobs 2
python submitOnBatch.py --input '../filelist/gg_HH_bbbb.txt'  --tag gg_HH_bb_kl_10_${TAG}     --kl 10      --xs 0.20894418     --njobs 2