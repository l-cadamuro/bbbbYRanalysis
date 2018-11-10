// c++ -lm -o skimNtuple skimNtuple.cpp `root-config --glibs --cflags` -I ../interface/ -lTreePlayer --std=c++17  -lTMVA
// ./skimNtuple ../filelist/gg_HH_bbbb.txt prova.root 0.01233  -1  [HH <klambda_value>]
//                           ^                 ^       ^       ^
//                         filelist         oname      xs[pb]  max nev 
#include "NtupleTrees.h"
#include "SkimEffCounter.h"
#include "OutputTree.h"
#include "HH_reweight.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include <iostream>
#include <cmath>
#include <array>
#include <utility>
#include <tuple>
#include <algorithm>
#include "TMVA/Reader.h"

#include <boost/program_options.hpp>
namespace po = boost::program_options;

using namespace std;

// make H1 and H2 as the pairs with the minimal mass diff between them

void d_to_f_vector(std::vector<double>& doubles, std::vector<float>& floats)
{
    for (uint i = 0; i < doubles.size(); ++i)
    {
        floats.at(i) = (float) doubles.at(i);
    }
    return;
}

std::array<TLorentzVector, 4> pair_jets_minmassdiff (TLorentzVector v1, TLorentzVector v2, TLorentzVector v3, TLorentzVector v4)
{
    vector < std::tuple<double, TLorentzVector, TLorentzVector, TLorentzVector, TLorentzVector > > configurations; // mass_diff, jets h1, jets h2
    TLorentzVector H1;
    TLorentzVector H2;
    double mass_diff;

    // 12 34
    // 13 24
    // 14 23

    H1 = v1 + v2;
    H2 = v3 + v4;
    mass_diff = std::fabs(H1.M() - H2.M());
    configurations.push_back(make_tuple(mass_diff, v1, v2, v3, v4));

    H1 = v1 + v3;
    H2 = v2 + v4;
    mass_diff = std::fabs(H1.M() - H2.M());
    configurations.push_back(make_tuple(mass_diff, v1, v3, v2, v4));


    H1 = v1 + v4;
    H2 = v2 + v3;
    mass_diff = std::fabs(H1.M() - H2.M());
    configurations.push_back(make_tuple(mass_diff, v1, v4, v2, v3));

    // take the one with the smallest mass diff
    sort(configurations.begin(), configurations.end(), [](
        const std::tuple<double, TLorentzVector, TLorentzVector, TLorentzVector, TLorentzVector > & a,
        const std::tuple<double, TLorentzVector, TLorentzVector, TLorentzVector, TLorentzVector > & b) -> bool
        {
            return (std::get<0>(a) <  std::get<0>(b));
        });

    std::array<TLorentzVector, 4> ojets = {
        std::get<1>(configurations.at(0)),
        std::get<2>(configurations.at(0)),
        std::get<3>(configurations.at(0)),
        std::get<4>(configurations.at(0))
    };
 
    return ojets;
}



int main(int argc, char** argv)
{
    cout << "... starting skim" << endl;

    // command line options - woudl be better with boost, but use old style input aergs to save hassle of linking boost

    string tname = string("ntuple");
    string fname;
    string oname;
    long long int maxEvts = -1;
    double xs = 1.0; // in pb
    bool do_postproc = true;
    bool is_HH = false;
    double klambda = 1.0;

    // ./skimNtuple inout output [maxEvts]
    if (argc > 2)
    {
        cout << "*** using cmd line values " << endl;
        fname = argv[1];
        oname = argv[2];
        if (argc > 3) xs = std::stod(argv[3]);
        if (argc > 4) maxEvts = std::stoi(argv[4]);
        if (argc > 5) is_HH   = ( string(argv[5]) == "HH" ? true : false);
        if (argc > 6) klambda = std::stod(argv[6]);
    }

    else
    {
        cout << "*** using default values " << endl;
        fname = "../filelist/gg_HH_bbbb.txt";
        oname = string("skim_ntuple_pt20.root");
    }

    cout << "*** doing postprocessing? " << std::boolalpha << do_postproc << std::noboolalpha << endl; 

    cout << " .. input  name : " << fname << endl;
    cout << " .. output name : " << oname << endl;
    cout << " .. sigma [pb]  : " << xs << endl;
    cout << " .. HH signal ? : " << std::boolalpha << is_HH << std::noboolalpha << endl;
    cout << " .. klambda     : " << klambda << endl;

    NtupleTrees nt (fname, tname);

    TFile* fOut = new TFile(oname.c_str(), "recreate");
    OutputTree ot("tree", "tree");
    SkimEffCounter ec;
    HH_reweight HHrew (
        "/afs/cern.ch/work/l/lcadamur/private/YR_bbbb_analysis/CMSSW_9_3_2/src/bbbbYRanalysis/data/Coefficients_14TeV.root",
        "/afs/cern.ch/work/l/lcadamur/private/YR_bbbb_analysis/CMSSW_9_3_2/src/bbbbYRanalysis/data/HH_SM_2D_histo.root"
    );


    // *********************************
    // ** parameters of skims
    const int btag_WP = 4; // medium WP with MTD
    const double min_pt_jet = 45.; // min jet pt . Assume trigger 100% efficiency for this
    const double max_eta_jet = 3.5; // max jet abs eta . Assume trigger 100% efficiency for this
    // *********************************

    // initialisations
    double tot_ev_sum = 0;
    double sel_ev_sum = 0;

    cout << "... running on max " << maxEvts << " events" << endl;

    for (long long int iEv = 0; true; ++iEv)
    {
        ot.clear();

        if (maxEvts > 0 && iEv >= maxEvts)
            break;

        if (!nt.Next())
            break;

        if (iEv % 10000 == 0)
            cout << " >> event " << iEv << endl;

        // look for gen HH information
        double w_HH_reweight = 1.0;
        if (is_HH)
        {
            double gen_mHH  ;
            double gen_costh;

            // loop through gen HH, and find the HH candidates

            int iH1 = -1;
            int iH2 = -1;

            TLorentzVector vH1;
            TLorentzVector vH2;

            for (uint ip = 0; ip < *(nt.get_Particle()->Particle_size); ++ip)
            {
                if (nt.get_Particle()->PID.At(ip) != 25) // only H
                    continue;
                if (nt.get_Particle()->Status.At(ip) != 22) // only status 22
                    continue;

                if (iH1 < 0)
                    iH1 = ip;
                else if (iH2 < 0){
                    iH2 = ip;
                    break; // it was checked that the case below never happens -> just save time here
                }
                // else
                //     cout << "** Event: " << iEv << " has > 2 H" << endl;
            }

            if (iH1 < 0 || iH2 < 0){
                cout << "***** Event: " << iEv << " did not find 2 H " <<  iH1 << " " << iH2 << endl;
                continue;
            }

            vH1.SetPtEtaPhiM(
                nt.get_Particle()->PT.At(iH1),
                nt.get_Particle()->Eta.At(iH1),
                nt.get_Particle()->Phi.At(iH1),
                nt.get_Particle()->Mass.At(iH1)
            );

            vH2.SetPtEtaPhiM(
                nt.get_Particle()->PT.At(iH2),
                nt.get_Particle()->Eta.At(iH2),
                nt.get_Particle()->Phi.At(iH2),
                nt.get_Particle()->Mass.At(iH2)
            );

            TLorentzVector vHH    = vH1 + vH2;
            TLorentzVector vHH_cm = vHH;
            auto boostv = vHH.BoostVector();
            vHH_cm.Boost(-boostv);

            TLorentzVector vH1_cm = vH1;
            TLorentzVector vH2_cm = vH2;
            vH1_cm.Boost(-boostv);
            vH2_cm.Boost(-boostv);

            gen_mHH   = vHH.M();
            gen_costh = vH1_cm.CosTheta();

            // HH found, now compute the weight
            w_HH_reweight = HHrew.get_weight(klambda, gen_mHH, gen_costh);
            // cout << "weight is " << w_HH_reweight << " fr kl, mHH, costh = " << klambda << " , " << gen_mHH << " , " << gen_costh << endl;

        } // end of the "if (is_HH)"


        double generator_w = nt.get_Event()->Weight.At(0); // first is the  event weight in Delphes
        
        // FIXME: here multiply any other weight that does not modify the total yield (it is mutliplied and summed in the evt_w). Example: HH reweight
        double evt_w = generator_w * w_HH_reweight;
        tot_ev_sum += evt_w; 
        ec.updateProcessed(evt_w);


        const int njets = *(nt.get_JetPUPPI()->JetPUPPI_size);
        // cout << "event has " << njets << " jets" <<endl;

        if (njets < 4) // at least 4 recosntructed jets
            continue;

        // int nbjets = 0;

        std::vector<int> preselected_jet_idx;

        // require 4 jets with min pt and b tag status.
        for (int ijet = 0; ijet < njets; ++ijet)
        {
            if (nt.get_JetPUPPI()->PT.At(ijet) < min_pt_jet)
                continue;

            if (std::fabs(nt.get_JetPUPPI()->Eta.At(ijet)) > max_eta_jet)
                continue;

            int tag_word = nt.get_JetPUPPI()->DeepCSV.At(ijet);
            bool is_tagged = (tag_word & (1 << btag_WP));

            if (!is_tagged)
                continue;

            // all good, save the jet
            preselected_jet_idx.push_back(ijet);
        }

        if (preselected_jet_idx.size() < 4) // at least 4 b tagged jets
            continue;

        // if there are more than 4 preselected jets, take the 4 highest pt ones

        int ij1 = preselected_jet_idx.at(0);
        int ij2 = preselected_jet_idx.at(1);
        int ij3 = preselected_jet_idx.at(2);
        int ij4 = preselected_jet_idx.at(3);

        TLorentzVector vj1 (0,0,0,0); 
        TLorentzVector vj2 (0,0,0,0); 
        TLorentzVector vj3 (0,0,0,0); 
        TLorentzVector vj4 (0,0,0,0); 

        vj1.SetPtEtaPhiM(
            nt.get_JetPUPPI()->PT.At(ij1),
            nt.get_JetPUPPI()->Eta.At(ij1),
            nt.get_JetPUPPI()->Phi.At(ij1),
            nt.get_JetPUPPI()->Mass.At(ij1)
            );
        vj2.SetPtEtaPhiM(
            nt.get_JetPUPPI()->PT.At(ij2),
            nt.get_JetPUPPI()->Eta.At(ij2),
            nt.get_JetPUPPI()->Phi.At(ij2),
            nt.get_JetPUPPI()->Mass.At(ij2)
            );
        vj3.SetPtEtaPhiM(
            nt.get_JetPUPPI()->PT.At(ij3),
            nt.get_JetPUPPI()->Eta.At(ij3),
            nt.get_JetPUPPI()->Phi.At(ij3),
            nt.get_JetPUPPI()->Mass.At(ij3)
            );
        vj4.SetPtEtaPhiM(
            nt.get_JetPUPPI()->PT.At(ij4),
            nt.get_JetPUPPI()->Eta.At(ij4),
            nt.get_JetPUPPI()->Phi.At(ij4),
            nt.get_JetPUPPI()->Mass.At(ij4)
            );

        // pair the jets into H1 and H2
        auto ordered_jets = pair_jets_minmassdiff(vj1, vj2, vj3, vj4);
        vj1 = ordered_jets.at(1 - 1);
        vj2 = ordered_jets.at(2 - 1);
        vj3 = ordered_jets.at(3 - 1);
        vj4 = ordered_jets.at(4 - 1);

        TLorentzVector vH1  = vj1 + vj2;
        TLorentzVector vH2  = vj3 + vj4;
        TLorentzVector vSum = vj1 + vj2 + vj3 + vj4;

        // update efficiency count
        sel_ev_sum += evt_w;
        ec.updateSelected(evt_w);

        // fill output tree properties
        ot.H1_pt    = vH1.Pt();
        ot.H1_eta   = vH1.Eta();
        ot.H1_phi   = vH1.Phi();
        ot.H1_mass  = vH1.M();
        
        ot.H2_pt    = vH2.Pt();
        ot.H2_eta   = vH2.Eta();
        ot.H2_phi   = vH2.Phi();
        ot.H2_mass  = vH2.M();

        ot.H1_b1_pt   = vj1.Pt();
        ot.H1_b1_eta  = vj1.Eta();
        ot.H1_b1_phi  = vj1.Phi();
        ot.H1_b1_mass = vj1.M();

        ot.H1_b2_pt   = vj2.Pt();
        ot.H1_b2_eta  = vj2.Eta();
        ot.H1_b2_phi  = vj2.Phi();
        ot.H1_b2_mass = vj2.M();

        ot.H2_b1_pt   = vj3.Pt();
        ot.H2_b1_eta  = vj3.Eta();
        ot.H2_b1_phi  = vj3.Phi();
        ot.H2_b1_mass = vj3.M();

        ot.H2_b2_pt   = vj4.Pt();
        ot.H2_b2_eta  = vj4.Eta();
        ot.H2_b2_phi  = vj4.Phi();
        ot.H2_b2_mass = vj4.M();

        ot.HH_pt    = vSum.Pt();
        ot.HH_eta   = vSum.Eta();
        ot.HH_mass  = vSum.M();
        ot.HH_mass_red_120 = vSum.M() - (vH1.M() - 120.) - (vH2.M() -120.);
        ot.HH_mass_red_125 = vSum.M() - (vH1.M() - 125.) - (vH2.M() -125.);


        ot.n_presel_jets = preselected_jet_idx.size();

        ot.evt_weight = evt_w;   // tnis gets normalised and accounts for efficiency ==> sum evt_w = 1
        ot.evt_scale  = xs; // this directly scales the event and increases the event yield

        // some control / study variables

        TLorentzVector vH1_sw  = vj1 + vj3; // the swapped candidates
        TLorentzVector vH2_sw  = vj2 + vj4; // the swapped candidates
        ot.H1_mass_sw  = vH1_sw.M();
        ot.H2_mass_sw  = vH2_sw.M();

        // variables for the BDT
        vector<int> idxs = {ij1, ij2, ij3, ij4};
        vector<double> dRs_jets;
        vector<double> detas_jets;
        vector<double> dphis_jets;
        
        for (uint li = 0; li < 4; ++li)
        {
            TLorentzVector tlv_ji = ordered_jets.at(li);
            for (uint lj = li+1; lj < 4; ++lj)
            {
                TLorentzVector tlv_jj = ordered_jets.at(lj);
                dRs_jets.push_back   (tlv_ji.DeltaR(tlv_jj)   );
                detas_jets.push_back (TMath::Abs(tlv_ji.Eta() - tlv_jj.Eta()) );
                dphis_jets.push_back (TMath::Abs(tlv_ji.DeltaPhi(tlv_jj)) );
            }
        }

        if (dRs_jets.size() != 6)
            cout << "** something strange with inputs to BDT? Size is " << dRs_jets.size() << endl;

        std::sort(dRs_jets.begin(), dRs_jets.end());
        std::sort(detas_jets.begin(), detas_jets.end());
        std::sort(dphis_jets.begin(), dphis_jets.end());


        ot.dR_jj_1 = dRs_jets.at(0); // min
        ot.dR_jj_2 = dRs_jets.at(1);
        ot.dR_jj_3 = dRs_jets.at(2);
        ot.dR_jj_4 = dRs_jets.at(3);
        ot.dR_jj_5 = dRs_jets.at(4);
        ot.dR_jj_6 = dRs_jets.at(5); // max

        ot.deta_jj_1 = detas_jets.at(0); // min
        ot.deta_jj_2 = detas_jets.at(1);
        ot.deta_jj_3 = detas_jets.at(2);
        ot.deta_jj_4 = detas_jets.at(3);
        ot.deta_jj_5 = detas_jets.at(4);
        ot.deta_jj_6 = detas_jets.at(5); // max

        ot.dphi_jj_1 = dphis_jets.at(0); // min
        ot.dphi_jj_2 = dphis_jets.at(1);
        ot.dphi_jj_3 = dphis_jets.at(2);
        ot.dphi_jj_4 = dphis_jets.at(3);
        ot.dphi_jj_5 = dphis_jets.at(4);
        ot.dphi_jj_6 = dphis_jets.at(5); // max

        ot.dphi_jj_H1   = vj1.DeltaPhi(vj2);
        ot.dphi_jj_H2   = vj3.DeltaPhi(vj4);
        ot.dtheta_jj_H1 = vj1.Eta() - vj2.Eta();
        ot.dtheta_jj_H2 = vj3.Eta() - vj4.Eta();
        ot.dR_jj_H1     = vj1.DeltaR(vj2);
        ot.dR_jj_H2     = vj3.DeltaR(vj4);

        ot.dR_HH    = vH1.DeltaR(vH2);
        ot.deta_HH  = vH1.Eta() - vH2.Eta();
        ot.dphi_HH  = vH1.DeltaPhi(vH2);

        // make the vectors in the CM
        auto boost_vctr = vSum.BoostVector(); // use -boost_vctr to go in the CM
        

        TLorentzVector vH1_cm = vH1;
        TLorentzVector vH2_cm = vH2;
        vH1_cm.Boost(-boost_vctr);
        vH2_cm.Boost(-boost_vctr);

        ot.costh_HH_H1_cm = 0; // cos theta in the CM between H1 and the HH system
        ot.costh_H1_cm = vH1_cm.CosTheta();    // cos theta in the CM of H1
        ot.costh_H2_cm = vH2_cm.CosTheta();    // cos theta in the CM of H2
        ot.costh_H1_j1_cm = 0; // cos theta in the CM of H1 and its first jet
        ot.costh_H2_j1_cm = 0; // cos theta in the CM of H2 and its first jet

        ot.HT = 0; //FIXME
        ot.ptsum_extra_jets = 0; //FIXME

        // check numnber of extra muons and electrons
        int n_ele     = 0;
        int n_ele_iso = 0;
        int n_ele_nooverlap     = 0;
        int n_ele_iso_nooverlap = 0;
        for (uint ie = 0; ie < *(nt.get_ElectronLoose()->ElectronLoose_size); ++ie)
        {
            TLorentzVector vEle;
            vEle.SetPtEtaPhiM(nt.get_ElectronLoose()->PT.At(ie), nt.get_ElectronLoose()->Eta.At(ie), nt.get_ElectronLoose()->Phi.At(ie), nt.get_ElectronLoose()->Mass.At(ie));
            bool e_presel = (nt.get_ElectronLoose()->PT.At(ie) > 7 && TMath::Abs(nt.get_ElectronLoose()->Eta.At(ie)) < 3.5);
            bool e_iso    = (nt.get_ElectronLoose()->IsolationVar.At(ie)  < 0.7);
            bool e_overlap = (
                vEle.DeltaR(vj1) < 0.5 ||
                vEle.DeltaR(vj2) < 0.5 ||
                vEle.DeltaR(vj3) < 0.5 ||
                vEle.DeltaR(vj4) < 0.5 
            );

            if (e_presel) ++ n_ele;
            if (e_presel && e_iso) ++ n_ele_iso;

            if (e_presel && !e_overlap) ++ n_ele_nooverlap;
            if (e_presel && e_iso && !e_overlap) ++ n_ele_iso_nooverlap;

        }
        ot.n_ele     = n_ele;
        ot.n_ele_iso = n_ele_iso;
        ot.n_ele_nooverlap     = n_ele_nooverlap;
        ot.n_ele_iso_nooverlap = n_ele_iso_nooverlap;

        int n_mu      = 0;
        int n_mu_iso  = 0;
        int n_mu_nooverlap      = 0;
        int n_mu_iso_nooverlap  = 0;
        for (uint imu = 0; imu < *(nt.get_MuonLoose()->MuonLoose_size); ++imu)
        {
            TLorentzVector vMu;
            vMu.SetPtEtaPhiM(nt.get_MuonLoose()->PT.At(imu), nt.get_MuonLoose()->Eta.At(imu), nt.get_MuonLoose()->Phi.At(imu), nt.get_MuonLoose()->Mass.At(imu));
            bool mu_presel = (nt.get_MuonLoose()->PT.At(imu) > 5 && TMath::Abs(nt.get_MuonLoose()->Eta.At(imu)) < 3.5);
            bool mu_iso    = (nt.get_MuonLoose()->IsolationVar.At(imu)  < 0.7);
            bool mu_overlap = (
                vMu.DeltaR(vj1) < 0.5 ||
                vMu.DeltaR(vj2) < 0.5 ||
                vMu.DeltaR(vj3) < 0.5 ||
                vMu.DeltaR(vj4) < 0.5 
            );

            if (mu_presel) ++ n_mu;
            if (mu_presel && mu_iso) ++ n_mu_iso;

            if (mu_presel && !mu_overlap) ++ n_mu_nooverlap;
            if (mu_presel && mu_iso && !mu_overlap) ++ n_mu_iso_nooverlap;

        }
        ot.n_mu     = n_mu;
        ot.n_mu_iso = n_mu_iso;
        ot.n_mu_nooverlap     = n_mu_nooverlap;
        ot.n_mu_iso_nooverlap = n_mu_iso_nooverlap;

        // finally, fill the output tree
        ot.fill();
    }

    double selection_eff    = sel_ev_sum/tot_ev_sum;
    double selection_eff_xc = ec.getSumWSelected()/ec.getSumWProcessed(); // cross check
    cout << "... selection efficiency is      " << selection_eff << " (" << selection_eff_xc << ")" << endl;
    cout << "... fraction of selected evts is " << 1.*ec.getNSelected()/ec.getNProcessed() << endl;

    ot.saveToFile(fOut);
    fOut->cd();
    ec.write();

    cout << "... skim done, now closing the input / output" << endl;
    fOut->Close();

    // --------------------------------------------------------------
    //
    // post processing: scale the MC weight, compute the BDT etc.

    if (do_postproc)
    {
        cout << "... doing the skim postprocessing" << endl;
        TFile * fOut_pp    = TFile::Open(oname.c_str(), "UPDATE");
        TTree * outTree_pp = (TTree*) fOut_pp->Get("tree");

        // create and initialize the readers
        // make a list of readers defined with (output branch name, MVA name, weight file name)
        // at the moment all readers must be trainings done on the same input variables
        std::vector<std::vector<string>> MVAs = {
            // {"BDTG_nomasscut",  "BDTG",   "/afs/cern.ch/work/l/lcadamur/private/YR_bbbb_analysis/CMSSW_9_3_2/src/bbbbYRanalysis/TMVA/dataset/weights/TMVAClassification_BDTG.weights.xml"},
            // //
            // {"BDTG_4_masscut",  "BDTG_4",   "/afs/cern.ch/work/l/lcadamur/private/YR_bbbb_analysis/CMSSW_9_3_2/src/bbbbYRanalysis/TMVA/dataset_masscut/weights/TMVAClassification_BDTG_4.weights.xml"},
            // {"BDTG_6_masscut",  "BDTG_6",   "/afs/cern.ch/work/l/lcadamur/private/YR_bbbb_analysis/CMSSW_9_3_2/src/bbbbYRanalysis/TMVA/dataset_masscut/weights/TMVAClassification_BDTG_6.weights.xml"},
            // {"BDTG_7_masscut",  "BDTG_7",   "/afs/cern.ch/work/l/lcadamur/private/YR_bbbb_analysis/CMSSW_9_3_2/src/bbbbYRanalysis/TMVA/dataset_masscut/weights/TMVAClassification_BDTG_7.weights.xml"},
            //
            {"BDTG_nomasscut",  "BDTG",   "/afs/cern.ch/work/l/lcadamur/private/YR_bbbb_analysis/CMSSW_9_3_2/src/bbbbYRanalysis/TMVA/dataset_allbkg/weights/TMVAClassification_BDTG.weights.xml"},
            //
            {"BDTG_4_masscut",  "BDTG_4",   "/afs/cern.ch/work/l/lcadamur/private/YR_bbbb_analysis/CMSSW_9_3_2/src/bbbbYRanalysis/TMVA/dataset_masscut_allbkg/weights/TMVAClassification_BDTG_4.weights.xml"},
            {"BDTG_6_masscut",  "BDTG_6",   "/afs/cern.ch/work/l/lcadamur/private/YR_bbbb_analysis/CMSSW_9_3_2/src/bbbbYRanalysis/TMVA/dataset_masscut_allbkg/weights/TMVAClassification_BDTG_6.weights.xml"},
            {"BDTG_7_masscut",  "BDTG_7",   "/afs/cern.ch/work/l/lcadamur/private/YR_bbbb_analysis/CMSSW_9_3_2/src/bbbbYRanalysis/TMVA/dataset_masscut_allbkg/weights/TMVAClassification_BDTG_7.weights.xml"},

        };

        std::vector<TMVA::Reader*> MVAreaders(MVAs.size());
        for (uint ir =0; ir < MVAs.size(); ++ir)
        {
            MVAreaders.at(ir) = new TMVA::Reader();
        }

        // ------------------------ now setup the variables
        std::vector<string> var_names = {
           "H1_mass",
           "H2_mass",
           "H1_pt",
           "H2_pt",
           "HH_mass",
           "HH_mass_red_125",
        // "n_presel_jets",
        // "dR_jj_1",
        // "dR_jj_2",
        // "dR_jj_3",
        // "dR_jj_4",
        // "dR_jj_5",
        // "dR_jj_6",
           "deta_jj_1",
        // "deta_jj_2",
        // "deta_jj_3",
        // "deta_jj_4",
        // "deta_jj_5",
           "deta_jj_6",
           "dphi_jj_1",
        // "dphi_jj_2",
        // "dphi_jj_3",
        // "dphi_jj_4",
        // "dphi_jj_5",
        // "dphi_jj_6",
           "dphi_jj_H1",
           "dphi_jj_H2",
           "dtheta_jj_H1",
           "dtheta_jj_H2",
           "dR_jj_H1",
           "dR_jj_H2",
           "dR_HH",
           "deta_HH",
           "dphi_HH",
           "costh_H1_cm",
        // "costh_H2_cm",
        };
        std::vector<double> var_values(var_names.size());
        std::vector<float>  var_values_fl(var_names.size()); // just the copy of the above, becasue shitty TMVA only wants floats in inputs

        // setup the readers
        for (uint iv = 0; iv < var_names.size(); ++iv)
        {
            string vname = var_names.at(iv);
            for (auto readr : MVAreaders)
            {
                readr->AddVariable (vname.c_str (), &(var_values_fl.at(iv)));
            }
        }

        // setup the address of the variables in the TTree
        for (uint iv = 0; iv < var_names.size(); ++iv)
        {
            outTree_pp->SetBranchAddress(var_names.at(iv).c_str(), &(var_values.at(iv)));
        }

        // book the MVAs
        for (uint imva = 0; imva < MVAs.size(); ++imva)
        {
            auto info = MVAs.at(imva);
            MVAreaders.at(imva) ->  BookMVA(info.at(1).c_str(), info.at(2).c_str());
        }

        // --------------------------------------------------


        // allocate new branches to the Tree
        double evt_weight_waccpt;
        TBranch *b_evt_weight_waccpt = outTree_pp->Branch ("evt_weight_waccpt", &evt_weight_waccpt) ;;

        // outputs of the MVAs will be stored here
        std::vector<double>   MVAoutputs(MVAs.size());
        std::vector<TBranch*> MVabranches(MVAs.size());
        for (uint io = 0; io < MVAoutputs.size(); ++io)
        {
            string bname = MVAs.at(io).at(0);
            cout << " >>> tree will have a new MVA branch named " << bname << endl;
            MVabranches.at(io) = outTree_pp->Branch(bname.c_str(), &MVAoutputs.at(io));
        }

        // readout the variables that are needed
        double evt_weight;
        outTree_pp->SetBranchAddress("evt_weight", &evt_weight);

        long long nEv_pp = outTree_pp->GetEntries();
        for (long long iEv_pp = 0; iEv_pp < nEv_pp; iEv_pp++)
        {
            outTree_pp->GetEntry(iEv_pp);
            d_to_f_vector(var_values, var_values_fl);

            // scale the MC weight for the 
            evt_weight_waccpt = evt_weight / ec.getSumWProcessed();
            for (uint imva = 0; imva < MVAreaders.size(); ++imva)
            {
                MVAoutputs.at(imva) = MVAreaders.at(imva)->EvaluateMVA(MVAs.at(imva).at(1).c_str());
            }

            // fill the extra bracnehs
            b_evt_weight_waccpt->Fill();
            for (auto br : MVabranches)
                br->Fill();
        }

        fOut_pp->cd();
        ec.write();
        outTree_pp->Write ("", TObject::kOverwrite) ;
        fOut_pp->Close();
    }

    cout << "... skim finished, exiting" << endl;

}