#include <iostream>
#include <vector>
#include <string>
#include <fstream>

// #include "TMVA/MsgLogger.h"
// #include "TMVA/Config.h"
// #include "TMVA/Tools.h"
// #include "TMVA/Factory.h"
// #include "TMVA/Reader.h"

#include "TMVA/Factory.h"
#include "TMVA/DataLoader.h"
#include "TMVA/Tools.h"
#include "TMVA/TMVAGui.h"

#include "TChain.h"
#include "TTree.h"
#include "TFile.h"
#include "TString.h"

using namespace std;

// c++ -lm -o training training.cpp `root-config --glibs --cflags` -lTMVA

int appendFromFileList (TChain* chain, string filename)
{
    //cout << "=== inizio parser ===" << endl;
    std::ifstream infile(filename.c_str());
    std::string line;
    int nfiles = 0;
    while (std::getline(infile, line))
    {
        line = line.substr(0, line.find("#", 0)); // remove comments introduced by #
        while (line.find(" ") != std::string::npos) line = line.erase(line.find(" "), 1); // remove white spaces
        while (line.find("\n") != std::string::npos) line = line.erase(line.find("\n"), 1); // remove new line characters
        while (line.find("\r") != std::string::npos) line = line.erase(line.find("\r"), 1); // remove carriage return characters
        if (!line.empty()) // skip empty lines
        {
            chain->Add(line.c_str());
            ++nfiles;
        }
     }
    return nfiles;
}


int main(int argc, char** argv)
// void training()
{
    bool mass_cut = false;
    string extratag = "allbkg";

    if (argc > 1)
    {
        if (std::stoi(argv[1]) == 1)
            mass_cut = true;
    }
    cout << "... apply mass cut? " << std::boolalpha << mass_cut << std::noboolalpha << endl;

    // create trees for signal and bkg
    string flist_HH = "../plotters/filelists/gg_HH_bbbb.txt";
    
    string flist_QCD_bEnriched_HT200to300   = "../plotters/filelists/bbbbTree_QCD_bEnriched_HT200to300.txt";
    string flist_QCD_bEnriched_HT300to500   = "../plotters/filelists/bbbbTree_QCD_bEnriched_HT300to500.txt";
    string flist_QCD_bEnriched_HT500to700   = "../plotters/filelists/bbbbTree_QCD_bEnriched_HT500to700.txt";
    string flist_QCD_bEnriched_HT700to1000  = "../plotters/filelists/bbbbTree_QCD_bEnriched_HT700to1000.txt";
    string flist_QCD_bEnriched_HT1000to1500 = "../plotters/filelists/bbbbTree_QCD_bEnriched_HT1000to1500.txt";
    string flist_QCD_bEnriched_HT1500to2000 = "../plotters/filelists/bbbbTree_QCD_bEnriched_HT1500to2000.txt";
    string flist_QCD_bEnriched_HT2000toInf  = "../plotters/filelists/bbbbTree_QCD_bEnriched_HT2000toInf.txt";
    //
    string flist_ttbar_inclusive     = "../plotters/filelists/ttbar_inclusive.txt";
    //
    string flist_ttHTobb             = "../plotters/filelists/ttHTobb.txt";
    string flist_ggZH_HToBB_ZToQQ    = "../plotters/filelists/ggZH_HToBB_ZToQQ.txt";
    string flist_ZH_HToBB_ZToQQ      = "../plotters/filelists/ZH_HToBB_ZToQQ.txt";
    string flist_WminusH_HToBB_WToQQ = "../plotters/filelists/WminusH_HToBB_WToQQ.txt";
    string flist_WplusH_HToBB_WToQQ  = "../plotters/filelists/WplusH_HToBB_WToQQ.txt";
    string flist_VBFHToBB            = "../plotters/filelists/VBFHToBB.txt";
    string flist_GluGluHToBB         = "../plotters/filelists/GluGluHToBB.txt";

    
    TChain* ch_sig = new TChain("tree");
    TChain* ch_bkg = new TChain("tree");

    // append to TChain
    int n_sig = 0;
    int n_bkg = 0;

    n_sig = appendFromFileList(ch_sig, flist_HH);

    n_bkg = appendFromFileList(ch_bkg, flist_QCD_bEnriched_HT200to300);
    n_bkg += appendFromFileList(ch_bkg, flist_QCD_bEnriched_HT300to500);
    n_bkg += appendFromFileList(ch_bkg, flist_QCD_bEnriched_HT500to700);
    n_bkg += appendFromFileList(ch_bkg, flist_QCD_bEnriched_HT700to1000);
    n_bkg += appendFromFileList(ch_bkg, flist_QCD_bEnriched_HT1000to1500);
    n_bkg += appendFromFileList(ch_bkg, flist_QCD_bEnriched_HT1500to2000);
    n_bkg += appendFromFileList(ch_bkg, flist_QCD_bEnriched_HT2000toInf);
    //
    n_bkg += appendFromFileList(ch_bkg, flist_ttbar_inclusive);
    //
    // n_bkg += appendFromFileList(ch_bkg, flist_ttHTobb);
    // n_bkg += appendFromFileList(ch_bkg, flist_ggZH_HToBB_ZToQQ);
    // n_bkg += appendFromFileList(ch_bkg, flist_ZH_HToBB_ZToQQ);
    // n_bkg += appendFromFileList(ch_bkg, flist_WminusH_HToBB_WToQQ);
    // n_bkg += appendFromFileList(ch_bkg, flist_WplusH_HToBB_WToQQ);
    // n_bkg += appendFromFileList(ch_bkg, flist_VBFHToBB);
    // n_bkg += appendFromFileList(ch_bkg, flist_GluGluHToBB);

    cout << "** INFO : Read " << n_sig << " signal files" << endl;
    cout << "** INFO : Read " << n_bkg << " background files" << endl;

    // room for the output
    
    string outputFileName = "tmva_training.root";
    if (mass_cut) outputFileName = "tmva_training_masscut.root";
    outputFileName += (string("_") + extratag);
    TFile * TMVA_outputfile = TFile::Open (outputFileName.c_str (),"RECREATE");

    // train signal vs bkg

    string variableTransformation="I:N";

    TMVA::Factory *TMVA_factory = new TMVA::Factory( "TMVAClassification", TMVA_outputfile,
        "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification"
    );

    // loader name will impact where the weihgts are stored
    // so change depedning on the trainign desired
    string dataloader_name = "dataset";
    if (mass_cut) dataloader_name = "dataset_masscut";
    dataloader_name += (string("_") + extratag);

    TMVA::DataLoader* TMVA_dataloader = new TMVA::DataLoader(dataloader_name);

    // TMVA::Factory * TMVA_factory = new TMVA::Factory (
    //     "tree", 
    //     TMVA_outputfile,
    //     Form (
    //     "!V:!Silent:%sColor:DrawProgressBar:AnalysisType=Classification%s", 
    //         gROOT->IsBatch ()?"!":"", 
    //         (":Transformations=" + variableTransformation).c_str ()
    //     )
    // );

    // define the training variables
    TMVA_dataloader->AddVariable("H1_mass", "H1_mass", "", 'F');
    TMVA_dataloader->AddVariable("H2_mass", "H2_mass", "", 'F');
    TMVA_dataloader->AddVariable("H1_pt", "H1_mass", "", 'F');
    TMVA_dataloader->AddVariable("H2_pt", "H2_mass", "", 'F');

    TMVA_dataloader->AddVariable("HH_mass", "HH_mass", "", 'F');
    TMVA_dataloader->AddVariable("HH_mass_red_125", "HH_mass_red_125", "", 'F');
    // TMVA_dataloader->AddVariable("n_presel_jets", "n_presel_jets", "", 'F');
    // TMVA_dataloader->AddVariable("dR_jj_1", "dR_jj_1", "", 'F');
    // TMVA_dataloader->AddVariable("dR_jj_2", "dR_jj_2", "", 'F');
    // TMVA_dataloader->AddVariable("dR_jj_3", "dR_jj_3", "", 'F');
    // TMVA_dataloader->AddVariable("dR_jj_4", "dR_jj_4", "", 'F');
    // TMVA_dataloader->AddVariable("dR_jj_5", "dR_jj_5", "", 'F');
    // TMVA_dataloader->AddVariable("dR_jj_6", "dR_jj_6", "", 'F');
    TMVA_dataloader->AddVariable("deta_jj_1", "deta_jj_1", "", 'F');
    // TMVA_dataloader->AddVariable("deta_jj_2", "deta_jj_2", "", 'F');
    // TMVA_dataloader->AddVariable("deta_jj_3", "deta_jj_3", "", 'F');
    // TMVA_dataloader->AddVariable("deta_jj_4", "deta_jj_4", "", 'F');
    // TMVA_dataloader->AddVariable("deta_jj_5", "deta_jj_5", "", 'F');
    TMVA_dataloader->AddVariable("deta_jj_6", "deta_jj_6", "", 'F');
    TMVA_dataloader->AddVariable("dphi_jj_1", "dphi_jj_1", "", 'F');
    // TMVA_dataloader->AddVariable("dphi_jj_2", "dphi_jj_2", "", 'F');
    // TMVA_dataloader->AddVariable("dphi_jj_3", "dphi_jj_3", "", 'F');
    // TMVA_dataloader->AddVariable("dphi_jj_4", "dphi_jj_4", "", 'F');
    // TMVA_dataloader->AddVariable("dphi_jj_5", "dphi_jj_5", "", 'F');
    // TMVA_dataloader->AddVariable("dphi_jj_6", "dphi_jj_6", "", 'F');
    TMVA_dataloader->AddVariable("dphi_jj_H1", "dphi_jj_H1", "", 'F');
    TMVA_dataloader->AddVariable("dphi_jj_H2", "dphi_jj_H2", "", 'F');
    TMVA_dataloader->AddVariable("dtheta_jj_H1", "dtheta_jj_H1", "", 'F');
    TMVA_dataloader->AddVariable("dtheta_jj_H2", "dtheta_jj_H2", "", 'F');
    TMVA_dataloader->AddVariable("dR_jj_H1", "dR_jj_H1", "", 'F');
    TMVA_dataloader->AddVariable("dR_jj_H2", "dR_jj_H2", "", 'F');
    TMVA_dataloader->AddVariable("dR_HH", "dR_HH", "", 'F');
    TMVA_dataloader->AddVariable("deta_HH", "deta_HH", "", 'F');
    TMVA_dataloader->AddVariable("dphi_HH", "dphi_HH", "", 'F');
    TMVA_dataloader->AddVariable("costh_H1_cm", "costh_H1_cm", "", 'F');
    // TMVA_dataloader->AddVariable("costh_H2_cm", "costh_H2_cm", "", 'F'); // 100% anticorrelato con il costh_H1
    

    // add the signal tree
    TMVA_dataloader->AddSignalTree    ( ch_sig, 1.0 );
    TMVA_dataloader->AddBackgroundTree( ch_bkg, 1.0 );

    // add the weights
    TMVA_dataloader->SetSignalWeightExpression     ( "evt_weight_waccpt * evt_scale" );
    TMVA_dataloader->SetBackgroundWeightExpression ( "evt_weight_waccpt * evt_scale" );
   
    // put here the training preselection
    TCut presel = "";
    if (mass_cut) presel = "TMath::Sqrt( (H1_mass - 120)*(H1_mass - 120) + (H2_mass - 120)*(H2_mass - 120) ) < 40";

    // FIXME: ottimizza la stringa qui
    TMVA_dataloader->PrepareTrainingAndTestTree( presel, presel,
        // "nTrain_Signal=1000:nTrain_Background=1000:SplitMode=Random:NormMode=NumEvents:!V" );
        "SplitMode=Random:!V" );

    // -----------------------------------------------------------------------------------------------------
    // -----------------------------------------------------------------------------------------------------

    // book the MVA methods

    // Gradient Boost
    TMVA_factory->BookMethod( TMVA_dataloader, TMVA::Types::kBDT, "BDTG",
                        "!H:!V:NTrees=1000:MinNodeSize=2.5%:BoostType=Grad:Shrinkage=0.10:UseBaggedBoost:BaggedSampleFraction=0.5:nCuts=20:MaxDepth=3" );

    TMVA_factory->BookMethod( TMVA_dataloader, TMVA::Types::kBDT, "BDTG_1",
                        "!H:!V:NTrees=500:MinNodeSize=2.5%:BoostType=Grad:Shrinkage=0.10:UseBaggedBoost:BaggedSampleFraction=0.5:nCuts=20:MaxDepth=3" );

    TMVA_factory->BookMethod( TMVA_dataloader, TMVA::Types::kBDT, "BDTG_2",
                        "!H:!V:NTrees=200:MinNodeSize=2.5%:BoostType=Grad:Shrinkage=0.10:UseBaggedBoost:BaggedSampleFraction=0.5:nCuts=20:MaxDepth=3" );

    TMVA_factory->BookMethod( TMVA_dataloader, TMVA::Types::kBDT, "BDTG_3",
                        "!H:!V:NTrees=1000:MinNodeSize=2.5%:BoostType=Grad:Shrinkage=0.10:UseBaggedBoost:BaggedSampleFraction=0.5:nCuts=20:MaxDepth=2" );

    TMVA_factory->BookMethod( TMVA_dataloader, TMVA::Types::kBDT, "BDTG_4",
                        "!H:!V:NTrees=500:MinNodeSize=2.5%:BoostType=Grad:Shrinkage=0.10:UseBaggedBoost:BaggedSampleFraction=0.5:nCuts=20:MaxDepth=2" );

    TMVA_factory->BookMethod( TMVA_dataloader, TMVA::Types::kBDT, "BDTG_5",
                        "!H:!V:NTrees=200:MinNodeSize=2.5%:BoostType=Grad:Shrinkage=0.10:UseBaggedBoost:BaggedSampleFraction=0.5:nCuts=20:MaxDepth=2" );

TMVA_factory->BookMethod( TMVA_dataloader, TMVA::Types::kBDT, "BDTG_6",
                        "!H:!V:NTrees=1000:MinNodeSize=2.5%:BoostType=Grad:Shrinkage=0.05:UseBaggedBoost:BaggedSampleFraction=0.5:nCuts=20:MaxDepth=3" );

    TMVA_factory->BookMethod( TMVA_dataloader, TMVA::Types::kBDT, "BDTG_7",
                        "!H:!V:NTrees=500:MinNodeSize=2.5%:BoostType=Grad:Shrinkage=0.05:UseBaggedBoost:BaggedSampleFraction=0.5:nCuts=20:MaxDepth=3" );

    TMVA_factory->BookMethod( TMVA_dataloader, TMVA::Types::kBDT, "BDTG_8",
                        "!H:!V:NTrees=200:MinNodeSize=2.5%:BoostType=Grad:Shrinkage=0.05:UseBaggedBoost:BaggedSampleFraction=0.5:nCuts=20:MaxDepth=3" );


    // Adaptive Boost
    // TMVA_factory->BookMethod( TMVA_dataloader, TMVA::Types::kBDT, "BDT",
    //                     "!H:!V:NTrees=850:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20" );

    // Bagging
    // TMVA_factory->BookMethod( TMVA_dataloader, TMVA::Types::kBDT, "BDTB",
    //                     "!H:!V:NTrees=400:BoostType=Bagging:SeparationType=GiniIndex:nCuts=20" );

    // Decorrelation + Adaptive Boost
    // TMVA_factory->BookMethod( TMVA_dataloader, TMVA::Types::kBDT, "BDTD",
    //                     "!H:!V:NTrees=400:MinNodeSize=5%:MaxDepth=3:BoostType=AdaBoost:SeparationType=GiniIndex:nCuts=20:VarTransform=Decorrelate" );

    // Allow Using Fisher discriminant in node splitting for (strong) linearly correlated variables
    // TMVA_factory->BookMethod( TMVA_dataloader, TMVA::Types::kBDT, "BDTF",
    //                     "!H:!V:NTrees=50:MinNodeSize=2.5%:UseFisherCuts:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:SeparationType=GiniIndex:nCuts=20" );


    // -----------------------------------------------------------------------------------------------------
    // -----------------------------------------------------------------------------------------------------
    
    // Train MVAs using the set of training events
    TMVA_factory->TrainAllMethods();
    
    // Evaluate all MVAs using the set of test events
    TMVA_factory->TestAllMethods();
    
    // Evaluate and compare performance of all configured MVAs
    TMVA_factory->EvaluateAllMethods();

    // -----------------------------------------------------------------------------------------------------
    // -----------------------------------------------------------------------------------------------------

    TMVA_outputfile->Close();
    std::cout << "==> Wrote root file: " << TMVA_outputfile->GetName() << std::endl;
    std::cout << "==> TMVAClassification is done!" << std::endl;
    delete TMVA_factory;
    delete TMVA_dataloader;
    
    // Launch the GUI for the root macros
    // if (!gROOT->IsBatch()) TMVA::TMVAGui( TMVA_outputfile );

    return 0;
}