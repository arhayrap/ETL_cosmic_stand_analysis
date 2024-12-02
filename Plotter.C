#include<iostream>
#include<typeinfo>

using namespace std;

void Plotter() {
    string offset = "1";
    ifstream file("offset_" + offset + "_files.txt");
    string time_delays[] = {"10", "15", "16", "17", "18", "19", "20"};
    string line;
    string base = "/home/aram/cosmic_data_analysis/ETROC_output/";
    // TCanvas * tot_vs_pixel = new TCanvas("tot_vs_pixel", "tot_vs_pixel", 1500, 1500);
    TCanvas * c = new TCanvas("tot_vs_pixel", "tot_vs_pixel", 1500, 1500);
    // tot_vs_pixel->Divide(3, 3);
    c->Divide(3, 3);
    gStyle->SetStatX(0.7);
    gStyle->SetStatY(0.9);
    int index = 0;
    while (getline(file, line)) {
        string title = "Time delay: " + time_delays[index] + ", offset: " + offset + ".";
        string input = base + line;
        string l = line;
        index++;
        cout<<input<<endl;
        TFile * f = TFile::Open(input.c_str());
        TTree * pulse = (TTree* )f -> Get("pulse");
        // tot_vs_pixel->cd(index); // Location on the canvas;
        c->cd(index); // Location on the canvas;
        // pulse->Draw("tot_code:cal_code", "", "colz");
        pulse->Draw("tot_code:(row*16+col)>>htemp(256,0,256,150,0,600)", "tot_code!=480&&cal_code>170&&cal_code<185", "colz");
        TH2D * htemp = (TH2D*)gPad->GetPrimitive("htemp");
        htemp->SetTitle(title.c_str());
        htemp->GetXaxis()->SetTitle("Pixel index");
        htemp->GetYaxis()->SetTitle("Time over threshold");
    }
    string output = "tot_code_pixel_" + offset + "_tot_peak_cal.png";
    c->SaveAs(output.c_str());
}