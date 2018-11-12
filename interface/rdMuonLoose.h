#ifndef RDMUONLOOSE_H
#define RDMUONLOOSE_H

#include "BaseTree.h"
#include "TChain.h"

class rdMuonLoose : public BaseTree {
    public:
        rdMuonLoose(TChain* ch);
        ~rdMuonLoose(){};

    TTreeReaderValue<Int_t>   MuonLoose_size  = {fReader, "MuonLoose_size"};
    TTreeReaderArray<Int_t>   Charge          = {fReader, "Charge"};
    TTreeReaderArray<Int_t>   Particle        = {fReader, "Particle"};
    TTreeReaderArray<Float_t> PT              = {fReader, "PT"};
    TTreeReaderArray<Float_t> Eta             = {fReader, "Eta"};
    TTreeReaderArray<Float_t> Phi             = {fReader, "Phi"};
    TTreeReaderArray<Float_t> Mass            = {fReader, "Mass"};
    TTreeReaderArray<Float_t> IsolationVar    = {fReader, "IsolationVar"};
    TTreeReaderArray<Float_t> Dz              = {fReader, "Dz"};
    TTreeReaderArray<Float_t> Dxy             = {fReader, "Dxy"};
    TTreeReaderArray<Float_t> SF              = {fReader, "SF"};
};

rdMuonLoose::rdMuonLoose(TChain* ch) :
    BaseTree(ch)
{}

#endif