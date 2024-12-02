#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <math.h>

using namespace std;

void Analysis(string filename) {
    
    string base = "/home/aram/cosmic_data_analysis/MergedData/";
    string full_path = base + filename;
    string output_file_path = "/home/aram/cosmic_data_analysis/Analysis/output_files/" + filename;
    cout<<"Reading the file"<<endl;
    TFile * file  = TFile::Open(full_path.c_str());
    TTree * pulse = (TTree * ) file -> Get("pulse");
    
    string print_variables[] = {
        "toa_code*3.125/cal_code", \
        "(tot_code*2 - floor(tot_code/32))*3.125/cal_code", \
        "cal_code", \
        "nhits", \
        "LP2_20[0]*1e9", \
        "LP2_20[1]*1e9", \
        "LP2_20[2]*1e9", \
        "LP2_20[7]*1e9", \
    };
    
    string condition = "tot_code!=480&&nhits!=0&&LP2_20[0]!=0.0&&LP2_20[1]!=0.0";
    
    string labels[] = {
        "TOA [ns]", \
        "TOT [ns]", \
        "CAL", \
        "N_{hits}", \
        "SiPm 1 (top) [ns]", \
        "SiPm 2 (bottom) [ns]", \
        "Photek [ns]", \
        "Clock [ns]", \
    };
    
    string histnames[] = {
        "TOA", \
        "TOT", \
        "CAL", \
        "Nhits", \
        "SiPm1_top", \
        "SiPm2_bottom", \
        "Photek", \
        "Clock", \
    };
    
    int n_vars = sizeof(print_variables) / sizeof(print_variables[0]);
    int c_cols = 4;
    int c_rows = ceil(n_vars*1.0 / c_cols);
    int plotsize = 400;
    int plotting_index = 0;
    TFile * output_file = new TFile(output_file_path.c_str(), "RECREATE");
    TCanvas * c = new TCanvas("Histograms", "Histograms", c_cols*plotsize, c_rows*plotsize);
    cout<<c_cols<<" "<<c_rows<<endl;
    c->Divide(c_cols, c_rows);
    for (int i = 0; i < n_vars; i++) { // 1D histograms
        plotting_index++;
        c->cd(plotting_index);
        string new_histname = histnames[i]; // "tmp_hist_"+to_string(i);
        string prefix = print_variables[i] +">>"+new_histname;
        pulse->Draw(prefix.c_str(), condition.c_str(), "");
        TH1D * tmp_hist = (TH1D*)gDirectory->Get(new_histname.c_str());
        tmp_hist->SetTitle("");
        tmp_hist->GetXaxis()->SetTitle(labels[i].c_str());
        tmp_hist->Write();
    }

    string print_variables_2D[] = {
        "toa_code*3.125/cal_code:(tot_code*2 - floor(tot_code/32))*3.125/cal_code", \
    };

    string histnames_2D[] = {
        "TOA_vs_TOT", \
    };

    string labels_x[] = {
        "TOA", \
    };

    string labels_y[] = {
        "TOT", \
    };

    n_vars = sizeof(print_variables_2D) / sizeof(print_variables_2D[0]);

    for (int i = 0; i < n_vars; i++) { // 2D histograms
        string new_histname = histnames_2D[i]; // "tmp_hist_"+to_string(i);
        string prefix = print_variables_2D[i] +">>"+new_histname;
        pulse->Draw(prefix.c_str(), condition.c_str(), "colz");
        TH2D * tmp_hist = (TH2D*)gDirectory->Get(new_histname.c_str());
        tmp_hist->SetTitle("");
        tmp_hist->GetXaxis()->SetTitle(labels_x[i].c_str());
        tmp_hist->GetYaxis()->SetTitle(labels_y[i].c_str());
        tmp_hist->Write();
    }

    output_file->Close();
}