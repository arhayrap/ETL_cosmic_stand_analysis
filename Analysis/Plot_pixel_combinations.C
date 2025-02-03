#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TLine.h"
#include "TCanvas.h"
#include <iostream>
#include "TStyle.h"

using namespace std;

void Plot_pixel_combinations() {
    
    TFile * f = new TFile("./output_files/merged_8_pixels.root");
    
    TH1D * DeltaT_pixel_0_0 = (TH1D*) f->Get("DeltaT_pixel_0_0");
    TH1D * DeltaT_pixel_0_1 = (TH1D*) f->Get("DeltaT_pixel_0_1");
    TH1D * DeltaT_pixel_1_0 = (TH1D*) f->Get("DeltaT_pixel_1_0");
    TH1D * DeltaT_pixel_1_1 = (TH1D*) f->Get("DeltaT_pixel_1_1");

    TCanvas * plot = new TCanvas("Plot", "Plot", 1200, 1200);
    gStyle->SetOptStat(0000);
    plot->Divide(2,2);

    plot->cd(1);
    DeltaT_pixel_0_0->SetTitle("Row < 8, Col < 8");
    DeltaT_pixel_0_0->GetXaxis()->SetTitle("#Delta T [ns]");
    DeltaT_pixel_0_0->Draw("HIST E");

    plot->cd(3);
    DeltaT_pixel_0_1->SetTitle("Row < 8, Col > 8");
    DeltaT_pixel_0_1->GetXaxis()->SetTitle("#Delta T [ns]");
    DeltaT_pixel_0_1->Draw("HIST E");

    plot->cd(2);
    DeltaT_pixel_1_0->SetTitle("Row > 8, Col < 8");
    DeltaT_pixel_1_0->GetXaxis()->SetTitle("#Delta T [ns]");
    DeltaT_pixel_1_0->Draw("HIST E");

    plot->cd(4);
    DeltaT_pixel_1_1->SetTitle("Row > 8, Col > 8");
    DeltaT_pixel_1_1->GetXaxis()->SetTitle("#Delta T [ns]");
    DeltaT_pixel_1_1->Draw("HIST E");

    plot->SaveAs("Pixel_8x8.png");
    plot->Close();

    f = new TFile("./output_files/merged_4_pixels.root");
    
    DeltaT_pixel_0_0 = (TH1D*) f->Get("DeltaT_pixel_0_0");
    DeltaT_pixel_0_1 = (TH1D*) f->Get("DeltaT_pixel_0_1");
    TH1D * DeltaT_pixel_0_2 = (TH1D*) f->Get("DeltaT_pixel_0_2");
    TH1D * DeltaT_pixel_0_3 = (TH1D*) f->Get("DeltaT_pixel_0_3");

    DeltaT_pixel_1_0 = (TH1D*) f->Get("DeltaT_pixel_1_0");
    DeltaT_pixel_1_1 = (TH1D*) f->Get("DeltaT_pixel_1_1");
    TH1D * DeltaT_pixel_1_2 = (TH1D*) f->Get("DeltaT_pixel_1_2");
    TH1D * DeltaT_pixel_1_3 = (TH1D*) f->Get("DeltaT_pixel_1_3");

    TH1D * DeltaT_pixel_2_0 = (TH1D*) f->Get("DeltaT_pixel_2_0");
    TH1D * DeltaT_pixel_2_1 = (TH1D*) f->Get("DeltaT_pixel_2_1");
    TH1D * DeltaT_pixel_2_2 = (TH1D*) f->Get("DeltaT_pixel_2_2");
    TH1D * DeltaT_pixel_2_3 = (TH1D*) f->Get("DeltaT_pixel_2_3");

    TH1D * DeltaT_pixel_3_0 = (TH1D*) f->Get("DeltaT_pixel_3_0");
    TH1D * DeltaT_pixel_3_1 = (TH1D*) f->Get("DeltaT_pixel_3_1");
    TH1D * DeltaT_pixel_3_2 = (TH1D*) f->Get("DeltaT_pixel_3_2");
    TH1D * DeltaT_pixel_3_3 = (TH1D*) f->Get("DeltaT_pixel_3_3");

    plot = new TCanvas("Plot", "Plot", 1200, 1200);
    gStyle->SetOptStat(0000);
    plot->Divide(4,4);

    plot->cd(1);
    DeltaT_pixel_0_0->SetTitle("Row < 4, Col < 4");
    DeltaT_pixel_0_0->GetXaxis()->SetTitle("#Delta T [ns]");
    DeltaT_pixel_0_0->Draw("HIST E");

    plot->cd(3);
    DeltaT_pixel_0_1->SetTitle("Row < 4, 8 > Col > 4");
    DeltaT_pixel_0_1->GetXaxis()->SetTitle("#Delta T [ns]");
    DeltaT_pixel_0_1->Draw("HIST E");

    plot->cd(2);
    DeltaT_pixel_0_2->SetTitle("Row < 4, 12 > Col > 8");
    DeltaT_pixel_0_2->GetXaxis()->SetTitle("#Delta T [ns]");
    DeltaT_pixel_0_2->Draw("HIST E");

    plot->cd(4);
    DeltaT_pixel_0_3->SetTitle("Row < 4, 16 > Col > 12");
    DeltaT_pixel_0_3->GetXaxis()->SetTitle("#Delta T [ns]");
    DeltaT_pixel_0_3->Draw("HIST E");
    
    // ----------------------------------------------------------
    
    plot->cd(5);
    DeltaT_pixel_1_0->SetTitle("8 > Row > 4, Col < 4");
    DeltaT_pixel_1_0->GetXaxis()->SetTitle("#Delta T [ns]");
    DeltaT_pixel_1_0->Draw("HIST E");

    plot->cd(6);
    DeltaT_pixel_1_1->SetTitle("8 > Row > 4, 8 > Col > 4");
    DeltaT_pixel_1_1->GetXaxis()->SetTitle("#Delta T [ns]");
    DeltaT_pixel_1_1->Draw("HIST E");

    plot->cd(7);
    DeltaT_pixel_1_2->SetTitle("8 > Row > 4, 12 > Col > 8");
    DeltaT_pixel_1_2->GetXaxis()->SetTitle("#Delta T [ns]");
    DeltaT_pixel_1_2->Draw("HIST E");

    plot->cd(8);
    DeltaT_pixel_1_3->SetTitle("8 > Row > 4, 16 > Col > 12");
    DeltaT_pixel_1_3->GetXaxis()->SetTitle("#Delta T [ns]");
    DeltaT_pixel_1_3->Draw("HIST E");
    
    // ----------------------------------------------------------
    
    plot->cd(9);
    DeltaT_pixel_2_0->SetTitle("12 > Row > 8, Col < 4");
    DeltaT_pixel_2_0->GetXaxis()->SetTitle("#Delta T [ns]");
    DeltaT_pixel_2_0->Draw("HIST E");

    plot->cd(10);
    DeltaT_pixel_2_1->SetTitle("12 > Row > 8, 8 > Col > 4");
    DeltaT_pixel_2_1->GetXaxis()->SetTitle("#Delta T [ns]");
    DeltaT_pixel_2_1->Draw("HIST E");

    plot->cd(11);
    DeltaT_pixel_2_2->SetTitle("12 > Row > 8, 12 > Col > 8");
    DeltaT_pixel_2_2->GetXaxis()->SetTitle("#Delta T [ns]");
    DeltaT_pixel_2_2->Draw("HIST E");

    plot->cd(12);
    DeltaT_pixel_2_3->SetTitle("12 > Row > 8, 16 > Col > 12");
    DeltaT_pixel_2_3->GetXaxis()->SetTitle("#Delta T [ns]");
    DeltaT_pixel_2_3->Draw("HIST E");
    
    // ----------------------------------------------------------
    
    plot->cd(13);
    DeltaT_pixel_3_0->SetTitle("16 > Row > 12, Col < 8");
    DeltaT_pixel_3_0->GetXaxis()->SetTitle("#Delta T [ns]");
    DeltaT_pixel_3_0->Draw("HIST E");

    plot->cd(14);
    DeltaT_pixel_3_1->SetTitle("16 > Row > 12, 8 > Col > 4");
    DeltaT_pixel_3_1->GetXaxis()->SetTitle("#Delta T [ns]");
    DeltaT_pixel_3_1->Draw("HIST E");

    plot->cd(15);
    DeltaT_pixel_3_2->SetTitle("16 > Row > 12, 12 > Col > 8");
    DeltaT_pixel_3_2->GetXaxis()->SetTitle("#Delta T [ns]");
    DeltaT_pixel_3_2->Draw("HIST E");

    plot->cd(16);
    DeltaT_pixel_3_3->SetTitle("16 > Row > 12, 16 > Col > 12");
    DeltaT_pixel_3_3->GetXaxis()->SetTitle("#Delta T [ns]");
    DeltaT_pixel_3_3->Draw("HIST E");
    
    // ----------------------------------------------------------
    
    plot->SaveAs("Pixel_4x4.png");
    plot->Close();
}