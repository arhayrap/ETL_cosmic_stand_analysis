#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TLine.h"
#include "TCanvas.h"
#include <iostream>
#include "TStyle.h"

using namespace std;

void Plotter() {
    
    TFile * f = new TFile("output.root");
    
    TH2D * MIN_TOT_NHITS_all = (TH2D*) f->Get("MIN_TOT_NHITS_all");
    TH2D * MIN_TOT_NHITS     = (TH2D*) f->Get("MIN_TOT_NHITS");
    TH2D * MAX_TOT_NHITS_all = (TH2D*) f->Get("MAX_TOT_NHITS_all");
    TH2D * MAX_TOT_NHITS     = (TH2D*) f->Get("MAX_TOT_NHITS");

    TH2D * HITMAP     = (TH2D*) f->Get("HEATMAP");
    TH2D * TOT_PIXEL  = (TH2D*) f->Get("TOT_PIXEL");
    TCanvas * plot = new TCanvas("Plot", "Plot", 1200, 1200);
    gStyle->SetOptStat(0000);
    plot->Divide(2,2);

    plot->cd(1);
    MIN_TOT_NHITS_all->Draw("colz");
    TLine * line = new TLine(0.0,1.0,25.0,1.0);
    line->SetLineWidth(2);
    line->SetLineColor(kRed);
    line->Draw();
    plot->cd(3);
    MIN_TOT_NHITS->Draw("colz");
    line->Draw();
    plot->cd(2);
    MAX_TOT_NHITS_all->Draw("colz");
    line->Draw();
    plot->cd(4);
    MAX_TOT_NHITS->Draw("colz");
    line->Draw();
    plot->SaveAs("TOT_vs_nhits.png");
    plot->Close();

    plot = new TCanvas("Plot", "Plot", 1200, 1200);
    plot->Divide(2,2);
    plot->cd(1);
    HITMAP->Draw("colz");
    plot->cd(2);
    TOT_PIXEL->GetXaxis()->SetRangeUser(50, 240);
    TOT_PIXEL->Draw("candlex3");
    plot->SaveAs("TOT_PIXEL.png");
    plot->Close();

}