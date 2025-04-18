#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <math.h>
#include "TMath.h"

using namespace std;

double Gaussian(double x, double mean, double sigma) {
    return (1/sigma*sqrt(2*TMath::Pi())) * TMath::Exp(-0.5 * pow((x - mean)/sigma, 2));
}

void Cal_vs_pixel(string filename) {
    string base = "../MergedData/";
    string full_path = base + filename;
    TFile * file = TFile::Open(full_path.c_str());
    TH2D * cal_vs_pixel_sum = new TH2D("cal_vs_pixel_sum", "cal_vs_pixel_sum", 16, 0, 15, 16, 0, 15);
    TH2D * elink_vs_pixel   = new TH2D("elink_vs_pixel", "elink_vs_pixel",     16, 0, 15, 16, 0, 15);
    TH2D * cal_vs_pixel_N   = new TH2D("cal_vs_pixel_N", "cal_vs_pixel_N",     16, 0, 15, 16, 0, 15);
    gStyle->SetOptStat(0000);
    TTree * pulse = (TTree *) file -> Get("pulse");
    const int n_init_hits = 100;
    int row[n_init_hits], col[n_init_hits], cal_code[n_init_hits], toa_code[n_init_hits], tot_code[n_init_hits], elink[n_init_hits];
    float amp[8];
    int nhits;

    pulse->SetBranchAddress("row", row);
    pulse->SetBranchAddress("col", col);
    pulse->SetBranchAddress("cal_code", cal_code);
    pulse->SetBranchAddress("toa_code", toa_code);
    pulse->SetBranchAddress("tot_code", tot_code);
    pulse->SetBranchAddress("amp", amp);
    pulse->SetBranchAddress("nhits", &nhits);
    pulse->SetBranchAddress("elink", &elink);

    for (int i = 0; i < pulse->GetEntries(); i++) {
        pulse->GetEntry(i);
        if (i%10000 == 0) std::cout<<i<<std::endl;
        // if (i > 10000) break;
        for (int j = 0; j < nhits; j++) {
            cal_code[j] = cal_code[j]*1.0;
            toa_code[j] = toa_code[j]*1.0;
            tot_code[j] = tot_code[j]*1.0;
            if (!(amp[2]>400&&amp[2]<1200&&cal_code[j]>170&&cal_code[j]<190&&(2*tot_code[j]-floor(tot_code[j]/32))*3.125/cal_code[j]>2&&(2*tot_code[j]-floor(tot_code[j]/32))*3.125/cal_code[j]<10)) continue;
            // if (row[j]==5 && col[j]==15) continue;
            // std::cout<<row[j]<<"   "<<col[j]<<"   "<<cal[j]<<std::endl;
            cal_vs_pixel_sum->SetBinContent(row[j]+1, col[j]+1, cal_vs_pixel_sum->GetBinContent(row[j]+1, col[j]+1) + cal_code[j]);
            elink_vs_pixel  ->SetBinContent(row[j]+1, col[j]+1, elink_vs_pixel  ->GetBinContent(row[j]+1, col[j]+1) + elink[j]);
            cal_vs_pixel_N  ->SetBinContent(row[j]+1, col[j]+1, cal_vs_pixel_N  ->GetBinContent(row[j]+1, col[j]+1) + 1);
        }
    }
    cal_vs_pixel_sum->SetMarkerSize(0.6);
    cal_vs_pixel_sum->SetTitle("");
    cal_vs_pixel_sum->Divide(cal_vs_pixel_N);
    TCanvas * c = new TCanvas("c", "c", 3000,2500);
    // cal_vs_pixel_sum->GetZaxis()->SetRangeUser(150,200);
    cal_vs_pixel_sum->GetXaxis()->SetTitle("Row");
    cal_vs_pixel_sum->GetYaxis()->SetTitle("Col");
    cal_vs_pixel_sum->Draw("colz text");
    c->SaveAs("cal_pixel.png");
    c = new TCanvas("hitmap", "hitmap", 3000,2500);
    cal_vs_pixel_N->Draw("colz text");
    c->SaveAs("hitmap.png");
    c = new TCanvas("elink", "elink", 3000,2500);
    elink_vs_pixel->Divide(cal_vs_pixel_N);
    elink_vs_pixel->Draw("colz text");
    c->SaveAs("elink.png");

}