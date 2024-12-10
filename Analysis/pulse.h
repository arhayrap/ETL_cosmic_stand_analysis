//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Dec  4 23:05:10 2024 by ROOT version 6.30/08
// from TTree pulse/
// found on file: ../MergedData_no_clock/check.root
//////////////////////////////////////////////////////////

#ifndef pulse_h
#define pulse_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class pulse {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   /*
   UInt_t          i_evt;
   Float_t         channel[8][1002];
   Float_t         time[1][1002];
   Float_t         baseline[8];
   Float_t         baseline_RMS[8];
   Float_t         noise[8];
   Float_t         amp[8];
   Float_t         t_peak[8];
   Float_t         integral[8];
   Float_t         intfull[8];
   Float_t         risetime[8];
   Float_t         decaytime[8];
   Float_t         LP2_5[8];
   Float_t         LP2_10[8];
   Float_t         LP2_15[8];
   Float_t         LP2_20[8];
   Float_t         LP2_25[8];
   Float_t         LP2_30[8];
   Float_t         LP2_35[8];
   Float_t         LP2_40[8];
   Float_t         LP2_50[8];
   Float_t         LP2_60[8];
   Float_t         LP2_70[8];
   Float_t         LP2_80[8];
   Float_t         LP2_30mV[8];
   Float_t         gaus_mean[8];
   Float_t         gaus_sigma[8];
   Float_t         gaus_chi2[8];
   Float_t         timeoffsets[8];
   */
   Int_t           event;
   Int_t           l1counter;
   Int_t           nrow;
   Int_t           row[256];   //[nrow]
   Int_t           ncol;
   Int_t           col[256];   //[ncol]
   Int_t           ntot_code;
   Int_t           tot_code[256];   //[ntot_code]
   Int_t           ntoa_code;
   Int_t           toa_code[256];   //[ntoa_code]
   Int_t           ncal_code;
   Int_t           cal_code[256];   //[ncal_code]
   Int_t           nelink;
   Int_t           elink[256];   //[nelink]
   Int_t           nchipid;
   Int_t           chipid[256];   //[nchipid]
   Int_t           bcid;
   Int_t           nnhits;
   Int_t           nhits[10];   //[nnhits]
   // Int_t           nhits;   //[nnhits]
   Int_t           nnhits_trail;
   Int_t           nhits_trail[1];   //[nnhits_trail]

   // List of branches
   /*
   TBranch        *b_i_evt;   //!
   TBranch        *b_channel;   //!
   TBranch        *b_time;   //!
   TBranch        *b_baseline;   //!
   TBranch        *b_baseline_RMS;   //!
   TBranch        *b_noise;   //!
   TBranch        *b_amp;   //!
   TBranch        *b_t_peak;   //!
   TBranch        *b_integral;   //!
   TBranch        *b_intfull;   //!
   TBranch        *b_risetime;   //!
   TBranch        *b_decaytime;   //!
   TBranch        *b_LP2_5;   //!
   TBranch        *b_LP2_10;   //!
   TBranch        *b_LP2_15;   //!
   TBranch        *b_LP2_20;   //!
   TBranch        *b_LP2_25;   //!
   TBranch        *b_LP2_30;   //!
   TBranch        *b_LP2_35;   //!
   TBranch        *b_LP2_40;   //!
   TBranch        *b_LP2_50;   //!
   TBranch        *b_LP2_60;   //!
   TBranch        *b_LP2_70;   //!
   TBranch        *b_LP2_80;   //!
   TBranch        *b_LP2_30mV;   //!
   TBranch        *b_gaus_mean;   //!
   TBranch        *b_gaus_sigma;   //!
   TBranch        *b_gaus_chi2;   //!
   TBranch        *b_timeoffsets;   //!
   */
   TBranch        *b_event;   //!
   TBranch        *b_l1counter;   //!
   TBranch        *b_nrow;   //!
   TBranch        *b_row;   //!
   TBranch        *b_ncol;   //!
   TBranch        *b_col;   //!
   TBranch        *b_ntot_code;   //!
   TBranch        *b_tot_code;   //!
   TBranch        *b_ntoa_code;   //!
   TBranch        *b_toa_code;   //!
   TBranch        *b_ncal_code;   //!
   TBranch        *b_cal_code;   //!
   TBranch        *b_nelink;   //!
   TBranch        *b_elink;   //!
   TBranch        *b_nchipid;   //!
   TBranch        *b_chipid;   //!
   TBranch        *b_bcid;   //!
   TBranch        *b_nnhits;   //!
   TBranch        *b_nhits;   //!
   TBranch        *b_nnhits_trail;   //!
   TBranch        *b_nhits_trail;   //!

   pulse(TTree *tree=0);
   virtual ~pulse();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef pulse_cxx
pulse::pulse(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("../MergedData/check.root");
      // TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("../ETROC_output/check.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("../MergedData/check.root");
         // f = new TFile("../ETROC_output/check.root");
      }
      f->GetObject("pulse",tree);

   }
   Init(tree);
}

pulse::~pulse()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t pulse::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t pulse::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void pulse::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   /*
   fChain->SetBranchAddress("i_evt", &i_evt, &b_i_evt);
   fChain->SetBranchAddress("channel", channel, &b_channel);
   fChain->SetBranchAddress("time", time, &b_time);
   fChain->SetBranchAddress("baseline", baseline, &b_baseline);
   fChain->SetBranchAddress("baseline_RMS", baseline_RMS, &b_baseline_RMS);
   fChain->SetBranchAddress("noise", noise, &b_noise);
   fChain->SetBranchAddress("amp", amp, &b_amp);
   fChain->SetBranchAddress("t_peak", t_peak, &b_t_peak);
   fChain->SetBranchAddress("integral", integral, &b_integral);
   fChain->SetBranchAddress("intfull", intfull, &b_intfull);
   fChain->SetBranchAddress("risetime", risetime, &b_risetime);
   fChain->SetBranchAddress("decaytime", decaytime, &b_decaytime);
   fChain->SetBranchAddress("LP2_5", LP2_5, &b_LP2_5);
   fChain->SetBranchAddress("LP2_10", LP2_10, &b_LP2_10);
   fChain->SetBranchAddress("LP2_15", LP2_15, &b_LP2_15);
   fChain->SetBranchAddress("LP2_20", LP2_20, &b_LP2_20);
   fChain->SetBranchAddress("LP2_25", LP2_25, &b_LP2_25);
   fChain->SetBranchAddress("LP2_30", LP2_30, &b_LP2_30);
   fChain->SetBranchAddress("LP2_35", LP2_35, &b_LP2_35);
   fChain->SetBranchAddress("LP2_40", LP2_40, &b_LP2_40);
   fChain->SetBranchAddress("LP2_50", LP2_50, &b_LP2_50);
   fChain->SetBranchAddress("LP2_60", LP2_60, &b_LP2_60);
   fChain->SetBranchAddress("LP2_70", LP2_70, &b_LP2_70);
   fChain->SetBranchAddress("LP2_80", LP2_80, &b_LP2_80);
   fChain->SetBranchAddress("LP2_30mV", LP2_30mV, &b_LP2_30mV);
   fChain->SetBranchAddress("gaus_mean", gaus_mean, &b_gaus_mean);
   fChain->SetBranchAddress("gaus_sigma", gaus_sigma, &b_gaus_sigma);
   fChain->SetBranchAddress("gaus_chi2", gaus_chi2, &b_gaus_chi2);
   fChain->SetBranchAddress("timeoffsets", timeoffsets, &b_timeoffsets);
   */
   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("l1counter", &l1counter, &b_l1counter);
   // fChain->SetBranchAddress("nrow", &nrow, &b_nrow);
   fChain->SetBranchAddress("row", row, &b_row);
   // fChain->SetBranchAddress("ncol", &ncol, &b_ncol);
   fChain->SetBranchAddress("col", col, &b_col);
   // fChain->SetBranchAddress("ntot_code", &ntot_code, &b_ntot_code);
   fChain->SetBranchAddress("tot_code", tot_code, &b_tot_code);
   // fChain->SetBranchAddress("ntoa_code", &ntoa_code, &b_ntoa_code);
   fChain->SetBranchAddress("toa_code", toa_code, &b_toa_code);
   // fChain->SetBranchAddress("ncal_code", &ncal_code, &b_ncal_code);
   fChain->SetBranchAddress("cal_code", cal_code, &b_cal_code);
   // fChain->SetBranchAddress("nelink", &nelink, &b_nelink);
   fChain->SetBranchAddress("elink", elink, &b_elink);
   // fChain->SetBranchAddress("nchipid", &nchipid, &b_nchipid);
   fChain->SetBranchAddress("chipid", chipid, &b_chipid);
   fChain->SetBranchAddress("bcid", &bcid, &b_bcid);
   // fChain->SetBranchAddress("nnhits", &nnhits, &b_nnhits);
   fChain->SetBranchAddress("nhits", nhits, &b_nhits);
   // fChain->SetBranchAddress("nnhits_trail", &nnhits_trail, &b_nnhits_trail);
   fChain->SetBranchAddress("nhits_trail", &nhits_trail, &b_nhits_trail);
   Notify();
}

Bool_t pulse::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void pulse::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t pulse::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef pulse_cxx
