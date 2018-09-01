// c++ -lm -o skimNtuple skimNtuple.cpp `root-config --glibs --cflags` -I ../interface/ -lTreePlayer --std=c++17

#include "NtupleTrees.h"
#include "OutputTree.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include <iostream>
#include <cmath>
#include <array>
#include <utility>
#include <tuple>
#include <algorithm>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

using namespace std;

// make H1 and H2 as the pairs with the minimal mass diff between them

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

    // ./skimNtuple inout output [maxEvts]
    if (argc > 2)
    {
        cout << "*** using cmd line values " << endl;
        fname = argv[1];
        oname = argv[2];
        if (argc > 3) maxEvts = std::stoi(argv[3]);
    }

    else
    {
        cout << "*** using default values " << endl;
        fname = "../filelist/gg_HH_bbbb.txt";
        oname = string("skim_ntuple_pt20.root");
    }

    cout << " .. input  name : " << fname << endl;
    cout << " .. output name : " << oname << endl;

    NtupleTrees nt (fname, tname);

    TFile* fOut = new TFile(oname.c_str(), "recreate");
    OutputTree ot("tree", "tree");

    // *********************************
    // ** parameters of skims
    const int btag_WP = 4; // medium WP with MTD
    const double min_pt_jet = 20.; // min jet pt . Assume trigger 100% efficiency for this
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

        double evt_w = 1.0; // FIXME: use here gen weights if necessary

        tot_ev_sum += evt_w; 

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

        // fill output tree properties
        ot.H1_pt    = vH1.Pt();
        ot.H1_eta   = vH1.Eta();
        ot.H1_mass  = vH1.M();
        
        ot.H2_pt    = vH2.Pt();
        ot.H2_eta   = vH2.Eta();
        ot.H2_mass  = vH2.M();

        ot.HH_pt    = vSum.Pt();
        ot.HH_eta   = vSum.Eta();
        ot.HH_mass  = vSum.M();

        ot.n_presel_jets = preselected_jet_idx.size();

        ot.fill();
    }

    double selection_eff = sel_ev_sum/tot_ev_sum;
    cout << "... selection efficiency is " << selection_eff << endl;

    ot.saveToFile(fOut);

    cout << "... skim finished, exiting" << endl;

}