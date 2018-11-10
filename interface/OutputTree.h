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
        double H1_phi;
        double H1_mass;

        double H2_pt;
        double H2_eta;
        double H2_phi;
        double H2_mass;

        // -----

        double H1_b1_pt;
        double H1_b1_eta;
        double H1_b1_phi;
        double H1_b1_mass;

        double H1_b2_pt;
        double H1_b2_eta;
        double H1_b2_phi;
        double H1_b2_mass;

        double H2_b1_pt;
        double H2_b1_eta;
        double H2_b1_phi;
        double H2_b1_mass;

        double H2_b2_pt;
        double H2_b2_eta;
        double H2_b2_phi;
        double H2_b2_mass;

        // -----

        double HH_pt;
        double HH_eta;
        double HH_phi;
        double HH_mass;
        double HH_mass_red_120;
        double HH_mass_red_125;

        // -----

        int n_presel_jets;
        int n_ele;
        int n_ele_iso;
        int n_ele_nooverlap;
        int n_ele_iso_nooverlap;
        int n_mu;
        int n_mu_iso;
        int n_mu_nooverlap;
        int n_mu_iso_nooverlap;

        // -----

        double evt_weight;
        // double evt_weight_waccpt; // also includes the acceptance (sum evt_weight_waccpt = 1)
        double evt_scale;

        // ----- for cross checks
        double H1_mass_sw;
        double H2_mass_sw;

        // ----- BDT variables
        double dR_jj_1; // min
        double dR_jj_2;
        double dR_jj_3;
        double dR_jj_4;
        double dR_jj_5;
        double dR_jj_6; // max

        double deta_jj_1; // min
        double deta_jj_2;
        double deta_jj_3;
        double deta_jj_4;
        double deta_jj_5;
        double deta_jj_6; // max

        double dphi_jj_1; // min
        double dphi_jj_2;
        double dphi_jj_3;
        double dphi_jj_4;
        double dphi_jj_5;
        double dphi_jj_6; // max

        double dphi_jj_H1;
        double dphi_jj_H2;
        double dtheta_jj_H1;
        double dtheta_jj_H2;
        double dR_jj_H1;
        double dR_jj_H2;

        double dR_HH;
        double deta_HH;
        double dphi_HH;

        double costh_HH_H1_cm; // cos theta in the CM between H1 and the HH system
        double costh_H1_cm;    // cos theta in the CM of H1
        double costh_H2_cm;    // cos theta in the CM of H2
        double costh_H1_j1_cm; // cos theta in the CM of H1 and its first jet
        double costh_H2_j1_cm; // cos theta in the CM of H2 and its first jet

        double HT;
        double ptsum_extra_jets;


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
    // delete tree_; // skitty ROOT.. tries to delete the tree for me once I save to disk
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
    tree_->Branch("H1_phi",  &H1_phi);
    tree_->Branch("H1_mass", &H1_mass);

    tree_->Branch("H2_pt",   &H2_pt);
    tree_->Branch("H2_eta",  &H2_eta);
    tree_->Branch("H2_phi",  &H2_phi);
    tree_->Branch("H2_mass", &H2_mass);

    tree_->Branch("H1_b1_pt",   &H1_b1_pt);
    tree_->Branch("H1_b1_eta",  &H1_b1_eta);
    tree_->Branch("H1_b1_phi",  &H1_b1_phi);
    tree_->Branch("H1_b1_mass", &H1_b1_mass);

    tree_->Branch("H1_b2_pt",   &H1_b2_pt);
    tree_->Branch("H1_b2_eta",  &H1_b2_eta);
    tree_->Branch("H1_b2_phi",  &H1_b2_phi);
    tree_->Branch("H1_b2_mass", &H1_b2_mass);

    tree_->Branch("H2_b1_pt",   &H2_b1_pt);
    tree_->Branch("H2_b1_eta",  &H2_b1_eta);
    tree_->Branch("H2_b1_phi",  &H2_b1_phi);
    tree_->Branch("H2_b1_mass", &H2_b1_mass);

    tree_->Branch("H2_b2_pt",   &H2_b2_pt);
    tree_->Branch("H2_b2_eta",  &H2_b2_eta);
    tree_->Branch("H2_b2_phi",  &H2_b2_phi);
    tree_->Branch("H2_b2_mass", &H2_b2_mass);

    tree_->Branch("HH_pt",   &HH_pt);
    tree_->Branch("HH_eta",  &HH_eta);
    tree_->Branch("HH_phi",  &HH_phi);
    tree_->Branch("HH_mass", &HH_mass);
    tree_->Branch("HH_mass_red_120", &HH_mass_red_120);
    tree_->Branch("HH_mass_red_125", &HH_mass_red_125);

    tree_->Branch("n_presel_jets", &n_presel_jets);
    tree_->Branch("n_ele",     &n_ele);
    tree_->Branch("n_ele_iso", &n_ele_iso);
    tree_->Branch("n_ele_nooverlap",     &n_ele_nooverlap);
    tree_->Branch("n_ele_iso_nooverlap", &n_ele_iso_nooverlap);
    tree_->Branch("n_mu",      &n_mu);
    tree_->Branch("n_mu_iso",  &n_mu_iso);
    tree_->Branch("n_mu_nooverlap",      &n_mu_nooverlap);
    tree_->Branch("n_mu_iso_nooverlap",  &n_mu_iso_nooverlap);

    tree_->Branch("evt_weight", &evt_weight);
    tree_->Branch("evt_scale",  &evt_scale);

    tree_->Branch("H1_mass_sw", &H1_mass_sw);
    tree_->Branch("H2_mass_sw", &H2_mass_sw);

    tree_->Branch("dR_jj_1",          &dR_jj_1);
    tree_->Branch("dR_jj_2",          &dR_jj_2);
    tree_->Branch("dR_jj_3",          &dR_jj_3);
    tree_->Branch("dR_jj_4",          &dR_jj_4);
    tree_->Branch("dR_jj_5",          &dR_jj_5);
    tree_->Branch("dR_jj_6",          &dR_jj_6);
    tree_->Branch("deta_jj_1",        &deta_jj_1);
    tree_->Branch("deta_jj_2",        &deta_jj_2);
    tree_->Branch("deta_jj_3",        &deta_jj_3);
    tree_->Branch("deta_jj_4",        &deta_jj_4);
    tree_->Branch("deta_jj_5",        &deta_jj_5);
    tree_->Branch("deta_jj_6",        &deta_jj_6);
    tree_->Branch("dphi_jj_1",        &dphi_jj_1);
    tree_->Branch("dphi_jj_2",        &dphi_jj_2);
    tree_->Branch("dphi_jj_3",        &dphi_jj_3);
    tree_->Branch("dphi_jj_4",        &dphi_jj_4);
    tree_->Branch("dphi_jj_5",        &dphi_jj_5);
    tree_->Branch("dphi_jj_6",        &dphi_jj_6);
    tree_->Branch("dphi_jj_H1",       &dphi_jj_H1);
    tree_->Branch("dphi_jj_H2",       &dphi_jj_H2);
    tree_->Branch("dtheta_jj_H1",     &dtheta_jj_H1);
    tree_->Branch("dtheta_jj_H2",     &dtheta_jj_H2);
    tree_->Branch("dR_jj_H1",         &dR_jj_H1);
    tree_->Branch("dR_jj_H2",         &dR_jj_H2);
    tree_->Branch("dR_HH",            &dR_HH);
    tree_->Branch("deta_HH",          &deta_HH);
    tree_->Branch("dphi_HH",          &dphi_HH);
    tree_->Branch("costh_HH_H1_cm",   &costh_HH_H1_cm);
    tree_->Branch("costh_H1_cm",      &costh_H1_cm);
    tree_->Branch("costh_H2_cm",      &costh_H2_cm);
    tree_->Branch("costh_H1_j1_cm",   &costh_H1_j1_cm);
    tree_->Branch("costh_H2_j1_cm",   &costh_H2_j1_cm);
    tree_->Branch("HT",               &HT);
    tree_->Branch("ptsum_extra_jets", &ptsum_extra_jets);

}

void OutputTree::clear()
{
    H1_pt   = -999.;
    H1_eta  = -999.;
    H1_phi  = -999.;
    H1_mass = -999.;

    H2_pt   = -999.;
    H2_eta  = -999.;
    H2_phi  = -999.;
    H2_mass = -999.;

    H1_b1_pt   = -999.;
    H1_b1_eta  = -999.;
    H1_b1_phi  = -999.;
    H1_b1_mass = -999.;

    H1_b2_pt   = -999.;
    H1_b2_eta  = -999.;
    H1_b2_phi  = -999.;
    H1_b2_mass = -999.;

    H2_b1_pt   = -999.;
    H2_b1_eta  = -999.;
    H2_b1_phi  = -999.;
    H2_b1_mass = -999.;

    H2_b2_pt   = -999.;
    H2_b2_eta  = -999.;
    H2_b2_phi  = -999.;
    H2_b2_mass = -999.;

    HH_pt   = -999.;
    HH_eta  = -999.;
    HH_phi  = -999.;
    HH_mass = -999.;
    HH_mass_red_120 = -999.;
    HH_mass_red_125 = -999.;

    n_presel_jets = 0;
    n_ele     = 0;
    n_ele_iso = 0;
    n_ele_nooverlap     = 0;
    n_ele_iso_nooverlap = 0;
    n_mu      = 0;
    n_mu_iso  = 0;
    n_mu_nooverlap      = 0;
    n_mu_iso_nooverlap  = 0;

    evt_weight = 0.0;
    evt_scale  = 0.0;

    H1_mass_sw = -999;
    H2_mass_sw = -999;

    dR_jj_1            = - 999.;
    dR_jj_2            = - 999.;
    dR_jj_3            = - 999.;
    dR_jj_4            = - 999.;
    dR_jj_5            = - 999.;
    dR_jj_6            = - 999.;
    deta_jj_1          = - 999.;
    deta_jj_2          = - 999.;
    deta_jj_3          = - 999.;
    deta_jj_4          = - 999.;
    deta_jj_5          = - 999.;
    deta_jj_6          = - 999.;
    dphi_jj_1          = - 999.;
    dphi_jj_2          = - 999.;
    dphi_jj_3          = - 999.;
    dphi_jj_4          = - 999.;
    dphi_jj_5          = - 999.;
    dphi_jj_6          = - 999.;
    dphi_jj_H1         = - 999.;
    dphi_jj_H2         = - 999.;
    dtheta_jj_H1       = - 999.;
    dtheta_jj_H2       = - 999.;
    dR_jj_H1           = - 999.;
    dR_jj_H2           = - 999.;
    dR_HH              = - 999.;
    deta_HH            = - 999.;
    dphi_HH            = - 999.;
    costh_HH_H1_cm     = - 999.;
    costh_H1_cm        = - 999.;
    costh_H2_cm        = - 999.;
    costh_H1_j1_cm     = - 999.;
    costh_H2_j1_cm     = - 999.;
    HT                 = - 999.;
    ptsum_extra_jets   = - 999.;
}