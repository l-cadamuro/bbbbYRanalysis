#ifndef SKIMEFFCOUNTER_H
#define SKIMEFFCOUNTER_H

/**
** class  : SkimEffCounter
** author : L. Cadamuro (UF)
** date   : 30/01/2018
** brief  : logs the numbner of processed events and the skim efficiency
**/

#include "TH1D.h"
#include <memory>
#include <iostream>

class SkimEffCounter {
    public:

        SkimEffCounter();
        ~SkimEffCounter(){};

        void updateProcessed (double evtW);
        void updateSelected  (double evtW);

        double   getSumWProcessed() {return Ntot_w;}
        long int getNProcessed()    {return Ntot_uw;}
        double   getSumWSelected()  {return Nsel_w;}
        long int getNSelected()     {return Nsel_uw;}

        void createHisto();
        int  write();

    private:
        enum BinValue{
            kNtot_w  = 1,
            kNtot_uw = 2,
            kNsel_w  = 3,
            kNsel_uw = 4,
        };
        std::unique_ptr<TH1D>  eff_histo_;

        // _w  : weighted
        // _uw : unweighted (plain ev count)
        
        double   Ntot_w;
        long int Ntot_uw;
        double   Nsel_w;
        long int Nsel_uw;

};


#include "SkimEffCounter.h"
using namespace std;

SkimEffCounter::SkimEffCounter()
{
    // extend with more bins if needed
    // eff_histo_ = std::unique_ptr<TH1D> (new TH1D("eff_histo", "eff_histo", 4, 0, 4));
    
    // eff_histo_->GetXaxis()->SetBinLabel(kNtot,            "Ntot");
    // eff_histo_->GetXaxis()->SetBinLabel(kNtot_unweighted, "Ntot_unweighted");
    // eff_histo_->GetXaxis()->SetBinLabel(kNsel,            "Nsel");
    // eff_histo_->GetXaxis()->SetBinLabel(kNsel_unweighted, "Nsel_unweighted");

    Ntot_w   = 0.0;
    Ntot_uw  = 0;
    Nsel_w   = 0.0;
    Nsel_uw  = 0;
}

void SkimEffCounter::updateProcessed (double evtW)
{
    Ntot_w  += evtW;
    Ntot_uw += 1;
}

void SkimEffCounter::updateSelected (double evtW)
{
    Nsel_w  += evtW;
    Nsel_uw += 1;
}

void SkimEffCounter::createHisto()
{
    if (eff_histo_)
    {
        cout << "[WARNING] SkimEffCounter : createHisto : histogram already exists, cannot create a new one" << endl;
        return;
    }
    eff_histo_ = std::unique_ptr<TH1D> (new TH1D("eff_histo", "eff_histo", 4, 0, 4));
    eff_histo_->SetDirectory(0);

    eff_histo_->SetBinContent(kNtot_w,  (double) getSumWProcessed());
    eff_histo_->SetBinContent(kNtot_uw, (double) getNProcessed());
    eff_histo_->SetBinContent(kNsel_w,  (double) getSumWSelected());
    eff_histo_->SetBinContent(kNsel_uw, (double) getNSelected());
    
    eff_histo_->GetXaxis()->SetBinLabel(kNtot_w,  "sumW_processed");
    eff_histo_->GetXaxis()->SetBinLabel(kNtot_uw, "N_processed");
    eff_histo_->GetXaxis()->SetBinLabel(kNsel_w,  "sumW_selected");
    eff_histo_->GetXaxis()->SetBinLabel(kNsel_uw, "N_selected");
}

int SkimEffCounter::write()
{
    if (!eff_histo_)
        createHisto();

    int result = eff_histo_->Write();
    eff_histo_->SetDirectory(0);
    return result;
    // return eff_histo_->Write();
}

#endif