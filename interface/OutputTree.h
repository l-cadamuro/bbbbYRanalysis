#include "TTree.h"
#include "TFile.h"
#include <string>

class OutputTree {
    
    public:
        OutputTree(std::string name, std::string title);
        ~OutputTree();
        void init();
        void clear();
        void fill() {tree_->Fill();}
        void saveToFile(TFile* fOut);

        // member variables
        
        double H1_pt;
        double H1_eta;
        double H1_mass;

        double H2_pt;
        double H2_eta;
        double H2_mass;

        double HH_pt;
        double HH_eta;
        double HH_mass;

        int n_presel_jets;

    private:
        TTree* tree_;
};

OutputTree::OutputTree(std::string name, std::string title)
{
    tree_ = new TTree(name.c_str(), title.c_str());

    init();
    clear();
}

OutputTree::~OutputTree()
{
    delete tree_;
}

void OutputTree::saveToFile(TFile* fOut)
{
    fOut->cd();
    tree_->Write();
}

void OutputTree::init()
{
    tree_->Branch("H1_pt",   &H1_pt);
    tree_->Branch("H1_eta",  &H1_eta);
    tree_->Branch("H1_mass", &H1_mass);

    tree_->Branch("H2_pt",   &H2_pt);
    tree_->Branch("H2_eta",  &H2_eta);
    tree_->Branch("H2_mass", &H2_mass);

    tree_->Branch("HH_pt",   &HH_pt);
    tree_->Branch("HH_eta",  &HH_eta);
    tree_->Branch("HH_mass", &HH_mass);

    tree_->Branch("n_presel_jets", &n_presel_jets);
}

void OutputTree::clear()
{
    H1_pt   = -999.;
    H1_eta  = -999.;
    H1_mass = -999.;

    H2_pt   = -999.;
    H2_eta  = -999.;
    H2_mass = -999.;

    HH_pt   = -999.;
    HH_eta  = -999.;
    HH_mass = -999.;

    n_presel_jets = 0;

}