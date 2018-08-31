// c++ -lm -o skimNtuple skimNtuple.cpp `root-config --glibs --cflags` -I ../interface/ -lTreePlayer

#include "NtupleTrees.h"
#include "OutputTree.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include <iostream>
#include <cmath>

using namespace std;

int main()
{
    cout << "... starting skim" << endl;

    string fname = "../filelist/gg_HH_bbbb.txt";
    string tname = string("ntuple");
    string oname = string("skim_ntuple.root");

    NtupleTrees nt (fname, tname);

    TFile* fOut = new TFile(oname.c_str(), "recreate");
    OutputTree ot("tree", "tree");

    // *********************************
    // ** parameters of skims
    const int btag_WP = 4; // medium WP with MTD
    const double min_pt_jet = 50.; // min jet pt . Assume trigger 100% efficiency for this
    const double max_eta_jet = 3.5; // max jet abs eta . Assume trigger 100% efficiency for this
    // *********************************

    // initialisations
    double tot_ev_sum = 0;
    double sel_ev_sum = 0;

    long long int maxEvts = 10000; // -1 for all
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

        // FIXME: pair the jets into H1 and H2

        TLorentzVector vSum = vj1 + vj2 + vj3 + vj4;

        // update efficiency count
        sel_ev_sum += evt_w;

        // fill output tree properties
        // FIXME: fill all
        ot.HH_mass = vSum.M();

        ot.n_presel_jets = preselected_jet_idx.size();

        ot.fill();
    }

    double selection_eff = sel_ev_sum/tot_ev_sum;
    cout << "... selection efficiency is " << selection_eff << endl;

    ot.saveToFile(fOut);

    cout << "... skim finished, exiting" << endl;

}