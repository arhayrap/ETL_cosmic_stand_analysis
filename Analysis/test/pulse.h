//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sun Dec  8 15:57:53 2024 by ROOT version 6.32.08
// from TTree pulse/pulse
// found on file: ../../ETROC_output/check.root
//////////////////////////////////////////////////////////

#ifndef pulse_h
#define pulse_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "vector"

class pulse {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           event;
   Int_t           l1counter;
   vector<int>     *row;
   vector<int>     *col;
   vector<int>     *tot_code;
   vector<int>     *toa_code;
   vector<int>     *cal_code;
   vector<int>     *elink;
   vector<int>     *chipid;
   Int_t           bcid;
   vector<int>     *nhits;
   vector<int>     *nhits_trail;

   // List of branches
   TBranch        *b_event;   //!
   TBranch        *b_l1counter;   //!
   TBranch        *b_row;   //!
   TBranch        *b_col;   //!
   TBranch        *b_tot_code;   //!
   TBranch        *b_toa_code;   //!
   TBranch        *b_cal_code;   //!
   TBranch        *b_elink;   //!
   TBranch        *b_chipid;   //!
   TBranch        *b_bcid;   //!
   TBranch        *b_nhits;   //!
   TBranch        *b_nhits_trail;   //!

   pulse(TTree *tree=0);
   virtual ~pulse();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual bool     Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef pulse_cxx
pulse::pulse(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("../../ETROC_output/check.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("../../ETROC_output/check.root");
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

   // Set object pointer
   row = 0;
   col = 0;
   tot_code = 0;
   toa_code = 0;
   cal_code = 0;
   elink = 0;
   chipid = 0;
   nhits = 0;
   nhits_trail = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("l1counter", &l1counter, &b_l1counter);
   fChain->SetBranchAddress("row", &row, &b_row);
   fChain->SetBranchAddress("col", &col, &b_col);
   fChain->SetBranchAddress("tot_code", &tot_code, &b_tot_code);
   fChain->SetBranchAddress("toa_code", &toa_code, &b_toa_code);
   fChain->SetBranchAddress("cal_code", &cal_code, &b_cal_code);
   fChain->SetBranchAddress("elink", &elink, &b_elink);
   fChain->SetBranchAddress("chipid", &chipid, &b_chipid);
   fChain->SetBranchAddress("bcid", &bcid, &b_bcid);
   fChain->SetBranchAddress("nhits", &nhits, &b_nhits);
   fChain->SetBranchAddress("nhits_trail", &nhits_trail, &b_nhits_trail);
   Notify();
}

bool pulse::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return true;
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
