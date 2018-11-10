#include "NtupleTrees.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include <vector>
#include <iostream>

using namespace std;

// c++ -lm -o find_ME_HH find_ME_HH.cpp `root-config --glibs --cflags` -lTreePlayer -I ../interface/

int main()
{
    cout << "... starting program" << endl;

    // command line options - woudl be better with boost, but use old style input aergs to save hassle of linking boost

    string tname = string("ntuple");
    string fname;
    string oname;
    long long int maxEvts = -1;
    // double xs = 1.0; // in pb
    fname = "../filelist/gg_HH_bbbb.txt";
    oname = string("HH_ME_info.root");

    cout << " .. input  name : " << fname << endl;
    cout << " .. output name : " << oname << endl;
    // cout << " .. sigma [pb]  : " << xs << endl;

    NtupleTrees nt (fname, tname);

    TFile* ofile = new TFile(oname.c_str(), "recreate");
    TTree* otree = new TTree("genHH", "genHH");

    double mHH; 
    double costh;
    double mH1; 
    double mH2;
    double H1pt; 
    double H2pt;
    double H1eta; 
    double H2eta;
    double H1phi; 
    double H2phi;

    otree->Branch("mHH", &mHH);
    otree->Branch("costh", &costh);
    otree->Branch("mH1", &mH1);
    otree->Branch("mH2", &mH2);
    otree->Branch("H1pt", &H1pt);
    otree->Branch("H2pt", &H2pt);
    otree->Branch("H1eta", &H1eta);
    otree->Branch("H2eta", &H2eta);
    otree->Branch("H1phi", &H1phi);
    otree->Branch("H2phi", &H2phi);

    for (long long int iEv = 0; true; ++iEv)
    {

        // clear outout
        mHH   = -999.;
        costh = -999.;
        mH1   = -999.;
        mH2   = -999.;
        H1pt  = -999.;
        H2pt  = -999.;
        H1eta = -999.;
        H2eta = -999.;
        H1phi = -999.;
        H2phi = -999.;

        if (maxEvts > 0 && iEv >= maxEvts)
            break;

        if (!nt.Next())
            break;

        if (iEv % 10000 == 0)
            cout << " >> event " << iEv << endl;

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
            else if (iH2 < 0)
                iH2 = ip;
            else
                cout << "** Event: " << iEv << " has > 2 H" << endl;
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

        mHH   = vHH.M();
        costh = vH1_cm.CosTheta();
        mH1   = vH1.M();
        mH2   = vH2.M();
        H1pt  = vH1.Pt();
        H2pt  = vH2.Pt();
        // H1eta = vH1.Eta(); // these are after the PDF x-boost
        // H2eta = vH2.Eta();
        H1eta = vH1_cm.Eta(); // these are before the PDF x-boost
        H2eta = vH2_cm.Eta();
        H1phi = vH1.Phi();
        H2phi = vH2.Phi();

        otree->Fill();
    }

    ofile->cd();
    otree->Write();
}