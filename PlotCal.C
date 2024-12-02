#include<iostream>
#include<typeinfo>

using namespace std;

void PlotCal() {
    string offset = "1";
    ifstream file("offset_" + offset + "_files.txt");
    string time_delays[] = {"10", "15", "16", "17", "18", "19", "20"};
    int colors[] = {2,3,4,5,6,7,8};
    string line;
    string base = "/home/aram/cosmic_data_analysis/ETROC_output/";
    // TCanvas * tot_vs_pixel = new TCanvas("tot_vs_pixel", "tot_vs_pixel", 1500, 1500);
    TCanvas * c = new TCanvas("tot", "tot", 1500, 1500);
    TLegend * l = new TLegend(0.8, 0.8, 0.9, 0.9);
    // tot_vs_pixel->Divide(3, 3);
    c->Divide(3, 3);
    gStyle->SetOptStat(0000);
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
        TTree * pulse = (TTree * ) f -> Get("pulse");
        // tot_vs_pixel->cd(index); // Location on the canvas;
        c->cd(index); // Location on the canvas;
        // pulse->Draw("tot_code>>htemp", "tot_code!=480", "colz");
        // pulse->Draw("cal_code>>htemp", "tot_code!=480", "colz");
        pulse->Draw("cal_code>>htemp", "tot_code!=480&&cal_code>170&&cal_code<185", "colz");
        // pulse->Draw("cal_code:tot_code>>htemp(100,0,500,30,100,200)", "", "colz");
        // pulse->Draw("((2*tot_code - floor(tot_code / 32)) * (3.125 / cal_code))>>htemp(100,0,50)", "", "");
        // pulse->Draw(">>nhits", "nhits>0", "nhits");
        // TEntryList * entryList = (TEntryList*)f->Get("pulse/nhits");
        // pulse->Draw("tot_code", "", "");
        // TH1D * htemp = (TH1D*)gPad->GetPrimitive("htemp");
        TH1D * htemp = (TH1D*)gDirectory->Get("htemp");
        /*
        htemp->Sumw2();
        htemp->Scale(1.0 / htemp->Integral());
        */
        // htemp->GetXaxis()->SetRangeUser(0, 100);
        // htemp->SetLineColor(colors[index]);
        htemp->SetLineWidth(2);
        htemp->SetTitle(title.c_str());
        // htemp->SetTitle("");
        htemp->GetXaxis()->SetTitle("Time over threshold");
        htemp->GetYaxis()->SetTitle("Fraction of events");
    }
    string output = "cal_code_" + offset + "_tot_peak_cal.png";
    c->SaveAs(output.c_str());
}