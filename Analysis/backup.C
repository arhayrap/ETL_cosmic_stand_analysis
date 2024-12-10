#include <iostream>
#include "TFile.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TTree.h"
#include "TStyle.h"

using namespace std;

std::string GetAllBranches(std::string filename) {
    TFile * f = TFile::Open(filename.c_str());
    TTree * t = (TTree *) f -> Get("atree");
    TObjArray * branches = t -> GetListOfLeaves();
    for (int b = 0; b < branches->GetEntries(); b++) {
        std::string leafname = branches->At(b)->GetName();
        if (leafname.startswith("Ach")) {
            std::cout<<leafname<<std::endl;
        }
    }
    return "";
}

void Plotter() {

    std::string base = "/uscms_data/d3/vvega/CMSSW_11_3_4/src/DRS4/NeuRad_tests/data/dataDSR4/20241205/analysis_ch";
    std::string prefix = "_trgn50mV_HV1800V.root";

    // std::string canvas_name = "Amplitudes " + std::to_string(channel);
    std::string canvas_name = "Amplitudes";
    // TCanvas * c_amp = new TCanvas(canvas_name.c_str(), canvas_name.c_str(), 1600, 800);
    // int colors[] = {2,3,4,6};
    // c_amp->Divide(4, 2);
    gStyle->SetOptStat(0000);
    std::string filename = base + std::to_string(1) + prefix;
    std::cout<<filename<<std::endl;
    GetAllBranches(filename);
}
