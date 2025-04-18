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
    /* if (sigma == 0) return 1.e30;
    Double_t arg = (x-mean)/sigma;
    Double_t res = TMath::Exp(-0.5*arg*arg);
    return res/(2.50662827463100024*sigma); //sqrt(2*Pi)=2.50662827463100024*/
    return (1/sigma*sqrt(2*TMath::Pi())) * TMath::Exp(-0.5 * pow((x - mean)/sigma, 2));
}

void Plotter(string filename) {
    
    string base = "./output_files/";
    string full_path = base + filename;
    TFile * file = TFile::Open(full_path.c_str());
    string variables[] = {"TOA", "TOT", "CAL", "HITMAP", "TOA_vs_Clock", "TOA_vs_TOT", "TOA_vs_MCP", "DeltaT_to_fit", "DeltaT_vs_TOT", "DeltaT_vs_TOT_corr", "DeltaT", "DeltaT_corr", "CAL_vs_pixelid", "TOT_vs_pixelid", "TOT_code", "DeltaT_corr"};
    
    const int nvars = sizeof(variables) / sizeof(variables[0]);
    for (int i = 0; i < nvars; i++) {
        TCanvas * c = new TCanvas(variables[i].c_str(), variables[i].c_str(), 1200, 700);
        if (variables[i] == "HITMAP" || variables[i].find("vs") != std::string::npos) { // 2D histograms
            TH2D * hist = (TH2D*) file->Get(variables[i].c_str());
            gStyle->SetOptStat(0000);
            if (variables[i] == "HITMAP") hist->Draw("colz text");
            else hist->Draw("colz");
        } else {
            gStyle->SetOptStat(1111);
            TH1D * hist = (TH1D*) file->Get(variables[i].c_str());
            if (variables[i] == "CAL") {
                c->SetLogy();
                hist->Draw();
            } else if (variables[i] == "DeltaT_to_fit" || variables[i] == "DeltaT_corr") {
                int max_bin = hist->GetMaximumBin();
                float max_bin_content = hist->GetBinContent(max_bin);
                float max = hist->GetXaxis()->GetBinCenter(max_bin);
                float mean = hist->GetMean();
                float std = hist->GetRMS();
                if (variables[i] == "DeltaT_to_fit") hist->Rebin(5);
                if (variables[i] == "DeltaT_corr") hist->Rebin(2);
                // TF1 * fit_function = new TF1("langaus", "[0]*Gaussian(x,[1],[2])", max - 2*std, max + 2*std);
                // TF1 * fit_function = new TF1("langaus", "[0]*TMath::Landau(x,[1],[2])", max - 2*std, max + 2*std);

                TF1 * fit_function = new TF1("langaus", "[0]*(TMath::Gaus(x,[1],[2],true)-TMath::Landau(x,[3],[4]))", max - 3*std, max + 3*std);
                // TF1 * fit_function = new TF1("langaus", "[0]*TMath::Gaus(x,[1],[2],true)", max - 1*std, max + 3*std);
                fit_function->SetParameters(max_bin_content, mean, std, mean, std);
                hist->GetXaxis()->SetTitle("#Delta T [ns]");
                hist->Draw();
                hist->Fit(fit_function, "0");
                gStyle->SetOptStat(1110);
                TPaveStats * stats = (TPaveStats*) hist -> FindObject("stats");
                stats->SetX1NDC(0.7);
                stats->SetX2NDC(0.9);
                stats->SetY1NDC(0.7);
                stats->SetY2NDC(0.9);
                gStyle->SetOptFit(111);
                fit_function->Draw("same");
            }
            else {
                hist->Draw();
            }
        }
        c->SaveAs(("output_plots/" + variables[i] + ".png").c_str());
    }
}