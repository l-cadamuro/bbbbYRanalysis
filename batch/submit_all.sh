TAG='10Set2018'

python submitOnBatch.py --input '../filelist/QCD_bEnriched_HT200to300.txt'   --tag QCD_bEnriched_HT200to300_${TAG}    --njobs 50    
python submitOnBatch.py --input '../filelist/QCD_bEnriched_HT300to500.txt'   --tag QCD_bEnriched_HT300to500_${TAG}    --njobs 50  
python submitOnBatch.py --input '../filelist/QCD_bEnriched_HT500to700.txt'   --tag QCD_bEnriched_HT500to700_${TAG}    --njobs 50  
python submitOnBatch.py --input '../filelist/QCD_bEnriched_HT700to1000.txt'  --tag QCD_bEnriched_HT700to1000_${TAG}   --njobs 50   
python submitOnBatch.py --input '../filelist/QCD_bEnriched_HT1000to1500.txt' --tag QCD_bEnriched_HT1000to1500_${TAG}  --njobs 50    
python submitOnBatch.py --input '../filelist/QCD_bEnriched_HT1500to2000.txt' --tag QCD_bEnriched_HT1500to2000_${TAG}  --njobs 50    
python submitOnBatch.py --input '../filelist/QCD_bEnriched_HT2000toInf.txt'  --tag QCD_bEnriched_HT2000toInf_${TAG}   --njobs 50   
