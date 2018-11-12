#ifndef RDELECTRONLOOSE_H
#define RDELECTRONLOOSE_H

#include "BaseTree.h"
#include "TChain.h"

class rdElectronLoose : public BaseTree {
    public:
        rdElectronLoose(TChain* ch);
        ~rdElectronLoose(){};

   TTreeReaderValue<Int_t>   ElectronLoose_size = {fReader, "ElectronLoose_size"};
   TTreeReaderArray<Int_t>   Charge             = {fReader, "Charge"};
   TTreeReaderArray<Int_t>   Particle           = {fReader, "Particle"};
   TTreeReaderArray<Float_t> PT                 = {fReader, "PT"};
   TTreeReaderArray<Float_t> Eta                = {fReader, "Eta"};
   TTreeReaderArray<Float_t> Phi                = {fReader, "Phi"};
   TTreeReaderArray<Float_t> Mass               = {fReader, "Mass"};
   TTreeReaderArray<Float_t> IsolationVar       = {fReader, "IsolationVar"};
   TTreeReaderArray<Float_t> TrackIso           = {fReader, "TrackIso"};
   TTreeReaderArray<Float_t> BDTScore           = {fReader, "BDTScore"};
   TTreeReaderArray<Float_t> Dz                 = {fReader, "Dz"};
   TTreeReaderArray<Float_t> Dxy                = {fReader, "Dxy"};
   TTreeReaderArray<Float_t> SF                 = {fReader, "SF"};
};

rdElectronLoose::rdElectronLoose(TChain* ch) :
    BaseTree(ch)
{}

#endif