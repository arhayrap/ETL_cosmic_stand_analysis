#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <math.h>
#include "TSystem.h"

using namespace std;

bool fileExists(const char* filePath) {
  return (gSystem->AccessPathName(filePath, kFileExists) == 0);
}

void PowerModeScan() {
    string base = "../ETROC_output_box_setup/";
    string mode[4] = {"default", "low", "medium", "high"};
    int colors[4] = {1, 3, 4, 8};
    string offset = "1";
    string pattern = "check_";
    string path = base + pattern + offset + "vth_";
    TFile * file[4] = {};
    TTree * trees[4] = {};

    for (int m = 0; m < 4; m++) {
        string full_path = path + mode[m] + ".root";
        if (fileExists(full_path.c_str())) {
            cout<<full_path<<endl;
            // file[m] = TFile::Open(full_path.c_str());
            if (mode[m] != "high") {
                // file[m] = TFile::Open("../ETROC_output_box_setup/merged_etroc.root");
                // file[m] = TFile::Open("../MergedData/check_merged_02_14_2025.root");
                file[m] = TFile::Open(full_path.c_str());
            } else {
                file[m] = TFile::Open("../ETROC_output_box_setup/all_files_new.root");
                // file[m] = TFile::Open(full_path.c_str());
            }
            trees[m] = (TTree * ) file[m] -> Get("pulse");
        }
    }

    string histnames[] = {
        "TOT", \
        "TOA", \
        "CAL", \
        "Nhits", \
    };

    string print_variables[] = {
        "(tot_code*2 - floor(tot_code/32))*3.125/cal_code", \
        "toa_code*3.125/cal_code", \
        "cal_code", \
        "nhits", \
    };

    string condition = "nhits>0&&cal_code>160&&cal_code<190"; // &&(2*tot_code-floor(tot_code/32))*3.125/cal_code>2

    string labels[] = {
        "TOT [ns]", \
        "TOA [ns]", \
        "CAL", \
        "N_{hits}", \
    };

    gStyle->SetOptStat(0000);
    float min = 0;
    float max = 0.2;
    bool log_scale = true;
    int n_vars = sizeof(labels) / sizeof(labels[0]);
    for (int v = 0; v < n_vars; v++) {
        TCanvas * c = new TCanvas(histnames[v].c_str(), histnames[v].c_str(), 1000, 1000);
        TLegend * l = new TLegend(0.7, 0.8, 1.0, 1.0);
        TH1D * tmp_hist[4] = {};
        for (int m = 0; m < 4; m++) {
            if (trees[m] == nullptr) continue;
            gROOT->cd();
            string full_path = path + mode[m];
            string histname_mode = histnames[v] + "_" + mode[m];
            string prefix = "";

            if (histnames[v] == "Nhits") {
                prefix = print_variables[v] + ">>" + histname_mode + "(50,0,50)";
            }
            else if (histnames[v] == "CAL") {
                prefix = print_variables[v] + ">>" + histname_mode + "(60,160,190)";
            }
            else {
                prefix = print_variables[v] + ">>" + histname_mode;
            }
            cout<<histname_mode<<endl;

            if (m == 0) {
                trees[m]->Draw(prefix.c_str(), condition.c_str(), "hist e");
            } else {
                trees[m]->Draw(prefix.c_str(), condition.c_str(), "hist e same");
            }
            tmp_hist[m] = (TH1D*) gDirectory->Get(histname_mode.c_str());
            tmp_hist[m]->SetTitle("");
            cout<<m<<"  "<<colors[m]<<endl;
            tmp_hist[m]->SetLineColor(colors[m]);
            tmp_hist[m]->SetLineWidth(3);
            tmp_hist[m]->GetYaxis()->SetTitle("Events");
            tmp_hist[m]->GetXaxis()->SetTitle(labels[v].c_str());
            cout<<tmp_hist[m]->Integral()<<endl;
            tmp_hist[m]->Scale(1.0/tmp_hist[m]->Integral());
            if (log_scale) min = 0.0001;

            if (histnames[v] == "Nhits") {
                tmp_hist[m]->GetYaxis()->SetRangeUser(0.0001, 1.0);
            } else {
                tmp_hist[m]->GetYaxis()->SetRangeUser(min, max);
            }
            /*
            if (m == 0) {
                cout<<"First"<<endl;
                tmp_hist[m]->Draw("hist e same");
            } else {
                cout<<"Second"<<endl;
                tmp_hist[m]->Draw("hist e same");
            }
            */
            c->SetLogy();
            c->Update();

            l->AddEntry(tmp_hist[m], mode[m].c_str());
        }
        c->Update();
        l->Draw("same");
        c->SaveAs((histnames[v]+".png").c_str());
    }
}