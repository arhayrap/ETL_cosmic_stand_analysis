#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TLine.h"
#include "TCanvas.h"
#include <iostream>
#include "TStyle.h"

using namespace std;

void Plot_LGAD_DT(string filename) {
    
    TFile * f = new TFile(("./output_files/" + filename).c_str());
    
    TH1D * DeltaT_pixel_0_0 = (TH1D*) f->Get("DeltaT_LGAD_1");
    TH1D * DeltaT_pixel_0_1 = (TH1D*) f->Get("DeltaT_LGAD_2");
    TH1D * DeltaT_pixel_1_0 = (TH1D*) f->Get("DeltaT_LGAD_3");
    TH1D * DeltaT_pixel_1_1 = (TH1D*) f->Get("DeltaT_LGAD_4");

    TCanvas * plot = new TCanvas("Plot", "Plot", 1200, 1200);
    gStyle->SetOptStat(1111);
    gStyle->SetOptFit(1111);

    plot->Divide(2,2);

    plot->cd(1);
    DeltaT_pixel_0_0->SetTitle("LGAD pixel: 1 1");
    DeltaT_pixel_0_0->GetXaxis()->SetTitle("#Delta T [ns]");
    TF1 * gaus1 = new TF1("Gaus1", "gaus", -0.07,0.12);
    DeltaT_pixel_0_0->Fit(gaus1);
    DeltaT_pixel_0_0->Draw("HIST E");
    gaus1->Draw("same");

    plot->cd(3);
    DeltaT_pixel_0_1->SetTitle("LGAD pixel: 1 2");
    DeltaT_pixel_0_1->GetXaxis()->SetTitle("#Delta T [ns]");
    TF1 * gaus2 = new TF1("Gaus2", "gaus", -0.07,0.12);
    // gaus2->SetFitRange()
    DeltaT_pixel_0_1->Fit(gaus2);
    DeltaT_pixel_0_1->Draw("HIST E");
    gaus2->Draw("same");

    plot->cd(2);
    DeltaT_pixel_1_0->SetTitle("LGAD pixel: 2 1");
    DeltaT_pixel_1_0->GetXaxis()->SetTitle("#Delta T [ns]");
    TF1 * gaus3 = new TF1("Gaus3", "gaus", -0.07,0.12);
    DeltaT_pixel_1_0->Fit(gaus3);
    DeltaT_pixel_1_0->Draw("HIST E");
    gaus3->Draw("same");

    plot->cd(4);
    DeltaT_pixel_1_1->SetTitle("LGAD pixel: 2 2");
    DeltaT_pixel_1_1->GetXaxis()->SetTitle("#Delta T [ns]");
    TF1 * gaus4 = new TF1("Gaus4", "gaus", -0.07,0.12);
    DeltaT_pixel_1_1->Fit(gaus4);
    DeltaT_pixel_1_1->Draw("HIST E");
    gaus4->Draw("same");

    plot->SaveAs("LGAD_DT.png");
    plot->Close();
}