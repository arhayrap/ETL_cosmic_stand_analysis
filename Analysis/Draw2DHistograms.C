#include <iostream>
#include "TFile.h"
#include "TCanvas.h"
#include "TH2D.h"
#include "TLegend.h"

void Draw2DHistograms() {
    // List of files to open
    std::vector<std::string> filepaths = {
        "output_tmp3.root", "output_tmp4.root", "output_tmp5.root", "output_tmp6.root", "output_tmp7.root" // Add your file paths here
    };

    // List of colors for each histogram
    std::vector<int> colors = {kRed, kBlue, kGreen, kMagenta, kOrange};

    // Create canvas
    TCanvas *c = new TCanvas("c", "Overlayed 2D Histograms", 800, 600);
    c->SetGrid();

    TLegend *legend = new TLegend(0.7, 0.7, 0.9, 0.9); // Legend to label the histograms

    // Loop over the files and retrieve histograms
    for (size_t i = 0; i < filepaths.size(); ++i) {
        TFile *file = TFile::Open(filepaths[i].c_str());
        if (!file || file->IsZombie()) {
            std::cerr << "Error: Could not open file " << filepaths[i] << std::endl;
            continue;
        }

        // Retrieve the 2D histogram (replace "hist2D" with your histogram name)
        TH2D *hist = (TH2D*)file->Get("TOT_PIXEL");
        if (!hist) {
            std::cerr << "Error: Could not retrieve histogram from file " << filepaths[i] << std::endl;
            file->Close();
            continue;
        }

        // Set histogram style
        hist->SetLineColor(colors[i % colors.size()]); // Use colors cyclically
        hist->SetLineWidth(2);

        // Draw the histogram
        if (i == 0) {
            hist->Draw("candle2x"); // First histogram sets the axes and scales
        } else {
            hist->Draw("candle2x same"); // Overlay subsequent histograms
        }

        // Add to legend
        legend->AddEntry(hist, filepaths[i].c_str(), "l");

        file->Close();
    }

    // Draw legend
    legend->Draw();

    // Save the canvas
    c->SaveAs("overlayed_2D_histograms.png");
}
