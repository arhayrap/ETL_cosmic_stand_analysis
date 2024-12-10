#define pulse_cxx
#include "pulse.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

int pixel_map[] = {57,65,77,81,82,83,97,98,99,113,114,193,194,200,216,232};

int GetIndex(int value) {
    int index = 0;
    for (int i = 0; i < sizeof(pixel_map) / sizeof(pixel_map[0]); i++) {
        if (value == (pixel_map[i]-1)) {
            index = i;
            // std::cout<<index<<"     "<<pixel_map[index]<<std::endl;
        }
    }
    return index;
}

void pulse::Loop()
{
//   In a ROOT session, you can do:
//      root> .L pulse.C
//      root> pulse t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;

   double cal_values[16][16] = {{182.0, 190.0, 190.0, 190.0, 191.0, 190.0, 186.0, 190.0, 186.0, 186.0, 188.0, 188.0, 187.0, 188.0, 186.0, 184.0},
                                {190.0, 189.0, 190.0, 190.0, 186.0, 186.0, 190.0, 189.0, 186.0, 187.0, 187.0, 188.0, 187.0, 187.0, 184.0, 186.0},
                                {190.0, 189.0, 190.0, 190.0, 192.0, 189.0, 188.0, 188.0, 186.0, 187.0, 692.0, 187.0, 187.0, 188.0, 186.0, 185.0},
                                {189.0, 189.0, 188.0, 190.0, 189.0, 185.0, 188.0, 188.0, 185.0, 186.0, 186.0, 185.0, 188.0, 187.0, 187.0, 186.0},
                                {188.0, 189.0, 190.0, 189.0, 188.0, 187.0, 186.0, 187.0, 184.0, 184.0, 177.0, 177.0, 187.0, 188.0, 187.0, 187.0},
                                {188.0, 189.0, 189.0, 187.0, 187.0, 187.0, 187.0, 188.0, 185.0, 183.0, 176.0, 688.0, 185.0, 187.0, 187.0, 186.0},
                                {188.0, 188.0, 187.0, 187.0, 186.0, 183.0, 187.0, 186.0, 182.0, 184.0, 183.0, 183.0, 177.0, 690.0, 186.0, 185.0},
                                {188.0, 186.0, 187.0, 187.0, 186.0, 186.0, 188.0, 185.0, 182.0, 174.0, 182.0, 176.0, 177.0, 177.0, 187.0, 688.0},
                                {183.0, 186.0, 186.0, 186.0, 186.0, 186.0, 187.0, 187.0, 182.0, 182.0, 174.0, 184.0, 177.0, 177.0, 178.0, 177.0},
                                {186.0, 186.0, 186.0, 181.0, 186.0, 186.0, 185.0, 186.0, 175.0, 182.0, 174.0, 183.0, 175.0, 176.0, 688.0, 176.0},
                                {186.0, 186.0, 181.0, 186.0, 187.0, 187.0, 186.0, 185.0, 182.0, 182.0, 182.0, 174.0, 175.0, 176.0, 174.0, 174.0},
                                {185.0, 186.0, 186.0, 185.0, 186.0, 185.0, 185.0, 185.0, 174.0, 181.0, 174.0, 173.0, 184.0, 183.0, 182.0, 173.0},
                                {184.0, 184.0, 183.0, 184.0, 184.0, 185.0, 184.0, 184.0, 180.0, 172.0, 172.0, 172.0, 173.0, 182.0, 180.0, 180.0},
                                {181.0, 181.0, 183.0, 183.0, 185.0, 180.0, 184.0, 184.0, 180.0, 172.0, 171.0, 182.0, 172.0, 181.0, 180.0, 180.0},
                                {180.0, 180.0, 182.0, 183.0, 184.0, 184.0, 184.0, 183.0, 180.0, 171.0, 180.0, 297.0, 172.0, 182.0, 180.0, 180.0},
                                {178.0, 178.0, 180.0, 181.0, 183.0, 183.0, 178.0, 183.0, 179.0, 180.0, 170.0, 180.0, 180.0, 181.0, 180.0, 180.0}};

   const int number_of_pixels = sizeof(pixel_map) / sizeof(pixel_map[0]);
   // std::cout<<number_of_pixels<<std::endl;
   int y_axis[number_of_pixels] = {};

   TFile * out_file = new TFile("output.root", "RECREATE");

   TH1D * TOT_all = new TH1D("TOT_all", "TOT_all", 100, 0, 20);
   TH1D * TOA_all = new TH1D("TOA_all", "TOA_all", 100, 0, 20);
   TH1D * CAL_all = new TH1D("CAL_all", "CAL_all", 250, 0, 250);
   TH1D * NHITS_all = new TH1D("NHITS_all", "NHITS_all", 50, 0, 50);
   TH2D * TOA_TOT_all = new TH2D("TOA_TOT_all", "TOA_TOT_all", 100, 0, 20, 100, 0, 20);
   TH2D * HEATMAP_all = new TH2D("HEATMAP_all", "HEATMAP_all", 16, 0, 16, 16, 0, 16);
   TH2D * TOT_PIXEL_all = new TH2D("TOT_PIXEL_all", "TOT_PIXEL_all", 256, 0, 256, 100, 0, 20);
   // TH2D * TOT_PIXEL_all = new TH2D("TOT_PIXEL_all", "TOT_PIXEL_all", number_of_pixels, 0, number_of_pixels, 100, 0, 20);
   TH1D * TOT = new TH1D("TOT", "TOT", 100, 0, 20);
   TH1D * TOA = new TH1D("TOA", "TOA", 100, 0, 20);
   TH1D * CAL = new TH1D("CAL", "CAL", 250, 0, 250);
   TH1D * NHITS = new TH1D("NHITS", "NHITS", 50, 0, 50);
   TH2D * TOA_TOT = new TH2D("TOA_TOT", "TOA_TOT", 100, 0, 20, 100, 0, 20);
   TH2D * HEATMAP = new TH2D("HEATMAP", "HEATMAP", 16, 0, 16, 16, 0, 16);
   // TH2D * TOT_PIXEL = new TH2D("TOT_PIXEL", "TOT_PIXEL", 256, 0, 256, 100, 0, 20);
   TH2D * TOT_PIXEL = new TH2D("TOT_PIXEL", "TOT_PIXEL", number_of_pixels, 0, number_of_pixels, 100, 0, 20);
   // TH2D * TOT_PIXEL = new TH2D("TOT_PIXEL", "TOT_PIXEL", 50, 0, 50, 100, 0, 20);
   TH2D * MEAN_TOT_NHITS_all = new TH2D("MEAN_TOT_NHITS_all", "MEAN_TOT_NHITS_all", 50, 0, 50, 100, 0, 20);
   TH2D * MEAN_TOT_NHITS = new TH2D("MEAN_TOT_NHITS", "MEAN_TOT_NHITS", 50, 0, 50, 100, 0, 20);
   TH2D * MIN_TOT_NHITS_all = new TH2D("MIN_TOT_NHITS_all", "MIN_TOT_NHITS_all", 25, 0, 25, 100, 0, 10);
   TH2D * MIN_TOT_NHITS = new TH2D("MIN_TOT_NHITS", "MIN_TOT_NHITS", 25, 0, 25, 100, 0, 10);
   TH2D * MAX_TOT_NHITS_all = new TH2D("MAX_TOT_NHITS_all", "MAX_TOT_NHITS_all", 25, 0, 25, 100, 0, 10);
   TH2D * MAX_TOT_NHITS = new TH2D("MAX_TOT_NHITS", "MAX_TOT_NHITS", 25, 0, 25, 100, 0, 10);

   TH1D * TOT_sel_pixels[number_of_pixels] = {};
   for (int n = 0; n < number_of_pixels; n++) {
       std::string name = "TOT_" + std::to_string(pixel_map[n]);
       TOT_sel_pixels[n] = new TH1D(name.c_str(), name.c_str(), 100, 0, 20);
   }

   for (Long64_t jentry=0; jentry<nentries; jentry++) {
      Long64_t ientry = LoadTree(jentry);
      // if (ientry > 100) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      cout<<jentry<<"/"<<nentries<<endl;
      // if (Cut(ientry) < 0) continue;
      int Nhits_all = 0;
      int Nhits = 0;
      double tot_mean_all = 0.0;
      double tot_mean = 0.0;
      double tot_min_all = 1000.0;
      double tot_min = 1000.0;
      double tot_max_all = 0.0;
      double tot_max = 0.0;
      for (int i = 0; i < nhits->at(0); i++) {
          Nhits_all+=1;
          if (tot_code->at(i) == 480) continue; // REMOVING THE UNEXPLAINED TOT VALUES
          double toa = toa_code->at(i) * 3.125 / cal_code->at(i);
          double tot = ( 2 * tot_code->at(i) - floor(tot_code->at(i) / 32) ) * (3.125 / cal_code->at(i));
          tot_mean_all+=tot;
          if (tot < tot_min_all) tot_min_all = tot;
          if (tot > tot_max_all) tot_max_all = tot;
          TOT_all->Fill(tot);
          TOA_all->Fill(toa);
          CAL_all->Fill(cal_code->at(i));
          TOA_TOT_all->Fill(toa, tot);
          HEATMAP_all->Fill(col->at(i), row->at(i));
          TOT_PIXEL_all->Fill((row->at(i)*16+col->at(i)), tot);
          // if (cal_code[i] == cal_values[row[i]][col[i]] && toa > 9 && toa < 10) { // Element level selections              Nhits+=1;
          if (cal_code->at(i) > 182 && cal_code->at(i) < 190 && tot > 2.5 && nhits->at(0) > 0) { // Element level selections
              bool skip_event = true;
              for (int b = 0; b < sizeof(pixel_map)/sizeof(pixel_map[0]); b++) {
                  if (std::round(row->at(i)*16+col->at(i))==(pixel_map[b]-1)) {
                      skip_event = false;
                  }
              }
              if (skip_event) continue;
              Nhits+=1;
              if (tot < tot_min) tot_min = tot;
              if (tot > tot_max) tot_max = tot;
              tot_mean+=tot;
              TOT->Fill(tot);
              TOA->Fill(toa);
              CAL->Fill(cal_code->at(i));
              TOA_TOT->Fill(toa, tot);
              HEATMAP->Fill(col->at(i), row->at(i)); // GetIndex(std::round(row[i]*16+col[i]))
              TOT_PIXEL->Fill(GetIndex(std::round(row->at(i)*16+col->at(i))), tot); // (row[i]*16+col[i]), tot);
              TOT_sel_pixels[GetIndex(std::round(row->at(i)*16+col->at(i)))] -> Fill(tot);
              // std::cout<<GetIndex(std::round(row[i]*16+col[i]))<<std::endl;
          }
      }
      MEAN_TOT_NHITS_all->Fill(Nhits_all, tot_mean_all/Nhits_all);
      MEAN_TOT_NHITS->Fill(Nhits, tot_mean/Nhits);
      if (Nhits_all>0){
          MIN_TOT_NHITS_all->Fill(Nhits_all, tot_min_all);
          MAX_TOT_NHITS_all->Fill(Nhits_all, tot_max_all);
      }
      if (Nhits>0) {
          MAX_TOT_NHITS->Fill(Nhits, tot_max);
          MIN_TOT_NHITS->Fill(Nhits, tot_min);
      }
      NHITS_all->Fill(Nhits_all);
      NHITS->Fill(Nhits);
   }
   TOT_PIXEL->GetXaxis()->SetNdivisions(2 * sizeof(pixel_map) / sizeof(pixel_map[0]));
   for (int k = 0; k <= 2 * sizeof(pixel_map) / sizeof(pixel_map[0]); k++) {
       if (k % 2 == 1) {
           TOT_PIXEL->GetXaxis()->ChangeLabel((k+1), 0, 0.02, -1, -1, -1, std::to_string(pixel_map[k/2]));
       } else {
           TOT_PIXEL->GetXaxis()->ChangeLabel((k+1), 0, 0, -1, -1, -1, "");
       }
   }

   TOT_all->Write();
   TOA_all->Write();
   CAL_all->Write();
   NHITS_all->Write();
   TOT->Write();
   TOA->Write();
   CAL->Write();
   NHITS->Write();

   TOA_TOT_all->Write();
   HEATMAP_all->Write();
   TOT_PIXEL_all->Write();
   TOA_TOT->Write();
   HEATMAP->Write();
   TOT_PIXEL->Write();
   MEAN_TOT_NHITS_all->Write();
   MEAN_TOT_NHITS->Write();
   MIN_TOT_NHITS_all->Write();
   MIN_TOT_NHITS->Write();
   MAX_TOT_NHITS_all->Write();
   MAX_TOT_NHITS->Write();

   for (int n = 0; n < number_of_pixels; n++) {
       TOT_sel_pixels[n] -> Write();
   }

   out_file->Close();
}
