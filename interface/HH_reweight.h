#ifndef HH_REWEIGHT_H
#define HH_REWEIGHT_H

#include <string>
#include <vector>
#include <array>
#include <utility>
#include <memory>

#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"

#define n_Ai_coeffs 15

class HH_reweight{
    
    public:
        HH_reweight (std::string coeff_file, std::string SM_distr_file);
        ~HH_reweight();
        double functionGF(double kl, double kt, double c2, double cg, double c2g, std::array<double, n_Ai_coeffs> const &A);
        double get_weight(double kl, double mHH, double costh);

    private:
        std::pair<int,int> get_bin_idx(double x, double y, TH2* histo);

        // member variables
        std::array<double, n_Ai_coeffs> A_integralXS = {
            2.100318379,
            10.2,
            0.287259045,
            0.098882779,
            1.321736614,
            -8.42431259,
            -1.388017366,
            2.8,
            0.518124457,
            -2.163473227,
            -0.550668596,
            5.871490593,
            0.296671491,
            -1.172793054,
            0.653429812
        };

        std::unique_ptr<TFile> f_14TeV_coeffs;
        std::unique_ptr<TFile> f_HH_14TeV_histo;
        TH2* HH_14TeV_histo;
        std::array<TH2*,n_Ai_coeffs> histos_A;
};

HH_reweight::HH_reweight(std::string coeff_file, std::string SM_distr_file)
{
    f_14TeV_coeffs   = std::unique_ptr<TFile> (new TFile(coeff_file.c_str()));
    f_HH_14TeV_histo = std::unique_ptr<TFile> (new TFile(SM_distr_file.c_str()));

    // read out the SM HH distribution
    HH_14TeV_histo = (TH2*) f_HH_14TeV_histo->Get("h_events_SM");

    // read out the Ai histogram coefficients
    for (uint idx = 0; idx < n_Ai_coeffs; ++idx)
    {
        int Ai = idx + 1; // there is a naming shift, first coefficient is A1, second is A2 etc...
        string hName = string("A") + std::to_string(Ai) + string("_14TeV");
        TH2D* h = (TH2D*) f_14TeV_coeffs->Get(hName.c_str());
        histos_A.at(idx) = h;
    }
}

HH_reweight::~HH_reweight()
{
    // will this doubly delete the ptrs?
    f_14TeV_coeffs->Close();
    f_HH_14TeV_histo->Close();
}

double HH_reweight::functionGF(double kl, double kt, double c2, double cg, double c2g, std::array<double, n_Ai_coeffs> const &A)
{
    return ( A[0]*pow(kt,4) + A[1]*pow(c2,2) + (A[2]*pow(kt,2) + A[3]*pow(cg,2))*pow(kl,2) + A[4]*pow(c2g,2) + ( A[5]*c2 + A[6]*kt*kl )*pow(kt,2) + (A[7]*kt*kl + A[8]*cg*kl )*c2 + A[9]*c2*c2g + (A[10]*cg*kl + A[11]*c2g)*pow(kt,2)+ (A[12]*kl*cg + A[13]*c2g )*kt*kl + A[14]*cg*c2g*kl );
}

double HH_reweight::get_weight(double kl, double mHH, double costh)
{
    auto bins = get_bin_idx(mHH, std::abs(costh), histos_A.at(0));
    std::array<double,n_Ai_coeffs> values_A;
    for (uint idx = 0; idx < n_Ai_coeffs; ++idx)
    {
        values_A.at(idx) = histos_A.at(idx)->GetBinContent(bins.first, bins.second); // read the 2D bin content for these mHH, costh values
        values_A.at(idx) = A_integralXS.at(idx)*values_A.at(idx);
    }

    double w = functionGF(kl, 1, 0, 0, 0, values_A);
    double SM_evts = HH_14TeV_histo->GetBinContent(bins.first, bins.second);
    double w_tot = w / SM_evts;
    return w_tot;
}


std::pair<int,int> HH_reweight::get_bin_idx(double x, double y, TH2* histo)
{
    int ix = histo->GetXaxis()->FindBin(x);
    int iy = histo->GetYaxis()->FindBin(y);
    if (ix < 1) ix = 1;
    if (iy < 1) iy = 1;
    if (ix > histo->GetNbinsX()) ix = histo->GetNbinsX();
    if (iy > histo->GetNbinsY()) iy = histo->GetNbinsY();
    return make_pair(ix,iy);
}

#endif