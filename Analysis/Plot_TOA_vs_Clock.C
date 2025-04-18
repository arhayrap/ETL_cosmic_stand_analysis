#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <math.h>
#include <TROOT.h>
#include <TKey.h>

using namespace std;

void Plot_TOA_vs_Clock() {

    std::string        base = "./output_files/";
    std::string filenames[] = {"run_809118316.root","run_809118416.root","run_809118516.root","run_809118616.root","run_809118716.root","run_809118816.root","run_809118916.root"};
    const int           n_f = sizeof(filenames) / sizeof(filenames[0]);
    TFile   *    files[n_f] = {};
    TH2D    *   histos[n_f] = {};
    TCanvas * canvases[n_f] = {};
    std::string labels[n_f] = {"Offset_3", "Offset_4", "Offset_5", "Offset_6", "Offset_7", "Offset_8", "Offset_9"};

    for (int i = 0; i < n_f; i++) {
        files[i] = TFile::Open((base + filenames[i]).c_str());
        if (files[i]->IsZombie()) continue;
        // if (files[i]->IsZombie()) continue;
        // gROOT->cd();
        // files[i]->cd();

        // Check the branches
        /*TKey *key;
        TIter nextkey(files[i]->GetListOfKeys());
        while ((key = (TKey*)nextkey())) {
            if (TString(key->GetClassName()).BeginsWith("TH1")) {
                TH1 *histogram = (TH1*)files[i]->Get(key->GetName());
                if (histogram) {
                    std::cout << "Histogram found: " << histogram->GetName() << std::endl;
                }
            }
        }*/

        // histos[i] = (TH2D *) files[i]->Get("TOT_vs_Clock");
        histos[i] = (TH2D *) files[i]->Get("TOA_vs_Clock");
        if (!histos[i]) continue;
        canvases[i] = new TCanvas(labels[i].c_str(), labels[i].c_str(), 1000, 1000);
        histos[i]->Draw("colz");
        canvases[i]->SaveAs((labels[i] + ".png").c_str());
    }
}