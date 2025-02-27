#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <math.h>
#include "TSystem.h"
#include "TChain.h"

using namespace std;

bool fileExists(const char* filePath) {
  return (gSystem->AccessPathName(filePath, kFileExists) == 0);
}

void ShiftScan() {
    std::string base = "../";
    std::string filenames[] = {"run_809018114000004121.root_i0.root","run_809018114000004151.root_i0.root","run_809018114000004181.root_i0.root","run_809018114000004211.root_i0.root","run_809018114000004241.root_i0.root","run_809018114000004271.root_i0.root","run_809018114000004301.root_i0.root","run_809018114000004361.root_i0.root"};
    int n_files = sizeof(filenames)/sizeof(filenames[0]);
    for (int i = 0; i < 60; i++) {
        int shift = -30 + i;
        TChain * chain = new TChain();
        TCanvas * c = new TCanvas("c", "c", 1000, 1000);
        for (int j = 0; j < n_files; j++) {
            std::string filename = (base + "MergedData_" + std::to_string(shift) + "/" + filenames[j] + "?#pulse");
            std::cout<<filename<<std::endl;
            // TFile * file = new TFile(filename.c_str());
            // TTree * tree = (TTree*) file->Get("pulse");
            // chain->Add(filename.c_str());
            chain->Add(filename.c_str());
            std::cout<<chain->GetEntries()<<endl;
        }
        chain->Draw(("(toa_code*3.125/cal_code):(Clock[0])>>tmp_hist"+std::to_string(shift)).c_str(), "nhits>0&&cal_code>160&&cal_code<190&&(toa_code*3.125/cal_code)<8", "colz");
        // TH2D * histogram = (TH2D * )
        c->Update();
        c->SaveAs(("./ShiftScanPlots/tmp_hist"+std::to_string(shift)+".png").c_str());
        c->Delete();
    }
}