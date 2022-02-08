//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Feb  7 15:39:48 2022 by ROOT version 6.24/06
// from TTree Neutron/Neutron
// found on file: ex_%1.1fMeV.root
//////////////////////////////////////////////////////////
#pragma once

#include <TCanvas.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1.h>
#include <TMath.h>
#include <TROOT.h>
#include <TStyle.h>

// Header file for the classes stored in the TTree if any.

class NeutronAna {
public:
  TTree *fChain;  //! pointer to the analyzed TTree or TChain
  Int_t fCurrent; //! current Tree number in a TChain

  // Fixed size dimensions of array or collections stored in the TTree if any.

  // Declaration of leaf types
  Double_t Energy;
  Double_t X;
  Double_t Y;
  Double_t Z;

  // Exitation energy of Deuterons
  double fExcitationE;

  // List of branches
  TBranch *b_Energy; //!
  TBranch *b_X;      //!
  TBranch *b_Y;      //!
  TBranch *b_Z;      //!

  NeutronAna(double exE, TTree *tree = 0);
  virtual ~NeutronAna();
  virtual Int_t Cut(Long64_t entry);
  virtual Int_t GetEntry(Long64_t entry);
  virtual Long64_t LoadTree(Long64_t entry);
  virtual void Init(TTree *tree);
  virtual void Loop();
  virtual TH1D *GetEneHist();
  virtual Bool_t Notify();
  virtual void Show(Long64_t entry = -1);
};

NeutronAna::NeutronAna(double excitationE, TTree *tree)
    : fChain(0), fExcitationE(excitationE) {
  // if parameter tree is not specified (or zero), connect the file
  // used to generate this class and read the Tree.
  if (tree == 0) {
    TFile *f = (TFile *)gROOT->GetListOfFiles()->FindObject(
        Form("ex_%1.1fMeV.root", fExcitationE));
    if (!f || !f->IsOpen()) {
      f = new TFile(Form("ex_%1.1fMeV.root", fExcitationE));
    }
    f->GetObject("Neutron", tree);
  }
  Init(tree);
}

NeutronAna::~NeutronAna() {
  if (!fChain)
    return;
  delete fChain->GetCurrentFile();
}

Int_t NeutronAna::GetEntry(Long64_t entry) {
  // Read contents of entry.
  if (!fChain)
    return 0;
  return fChain->GetEntry(entry);
}
Long64_t NeutronAna::LoadTree(Long64_t entry) {
  // Set the environment to read one entry
  if (!fChain)
    return -5;
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0)
    return centry;
  if (fChain->GetTreeNumber() != fCurrent) {
    fCurrent = fChain->GetTreeNumber();
    Notify();
  }
  return centry;
}

void NeutronAna::Init(TTree *tree) {
  // The Init() function is called when the selector needs to initialize
  // a new tree or chain. Typically here the branch addresses and branch
  // pointers of the tree will be set.
  // It is normally not necessary to make changes to the generated
  // code, but the routine can be extended by the user if needed.
  // Init() will be called many times when running on PROOF
  // (once per file to be processed).

  // Set branch addresses and branch pointers
  if (!tree)
    return;
  fChain = tree;
  fCurrent = -1;
  fChain->SetMakeClass(1);

  fChain->SetBranchAddress("Energy", &Energy, &b_Energy);
  fChain->SetBranchAddress("X", &X, &b_X);
  fChain->SetBranchAddress("Y", &Y, &b_Y);
  fChain->SetBranchAddress("Z", &Z, &b_Z);
  Notify();
}

Bool_t NeutronAna::Notify() {
  // The Notify() function is called when a new file is opened. This
  // can be either for a new TTree in a TChain or when when a new TTree
  // is started when using PROOF. It is normally not necessary to make changes
  // to the generated code, but the routine can be extended by the
  // user if needed. The return value is currently not used.

  return kTRUE;
}

void NeutronAna::Show(Long64_t entry) {
  // Print contents of entry.
  // If entry is not specified, print current entry
  if (!fChain)
    return;
  fChain->Show(entry);
}
Int_t NeutronAna::Cut(Long64_t entry) {
  // This function may be called from Loop.
  // returns  1 if entry is accepted.
  // returns -1 otherwise.
  return 1;
}

Double_t *GetLinBin(Int_t NBinX = 100, Double_t minX = 0.,
                    Double_t maxX = 50.) {
  Double_t *BinX_lin = new Double_t[NBinX + 1];
  Double_t step = (maxX - minX) / (double)NBinX;
  for (int i = 0; i < NBinX + 1; ++i) {
    BinX_lin[i] = minX + i * step;
  }

  return BinX_lin;
}

Double_t *GetLogBin(Int_t NBinX = 100, Double_t minX = 1E-3,
                    Double_t maxX = 1E2) {
  Double_t *BinX_log = new Double_t[NBinX + 1];
  Double_t minX_log = TMath::Log10(minX);
  Double_t maxX_log = TMath::Log10(maxX);
  Double_t step_in_log = (maxX_log - minX_log) / (double)NBinX;
  for (int i = 0; i < NBinX + 1; ++i) {
    Double_t tmp = minX_log + ((double)i) * step_in_log;
    Double_t val = TMath::Power(10., tmp);
    BinX_log[i] = val;
  }

  return BinX_log;
}

void NeutronAna::Loop() {
  //   In a ROOT session, you can do:
  //      root> .L NeutronAna.C
  //      root> NeutronAna t
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
  // by  b_branchname->GetEntry(ientry); //read only this branch
  if (fChain == 0)
    return;

  Long64_t nentries = fChain->GetEntriesFast();

  int N = 100;
  TH1D *h = new TH1D("h", "", N, GetLogBin(N));

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry = 0; jentry < nentries; jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0)
      break;
    nb = fChain->GetEntry(jentry);
    nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    h->Fill(Energy);
  }

  h->Draw();
}

TH1D *NeutronAna::GetEneHist() {
  //   In a ROOT session, you can do:
  //      root> .L NeutronAna.C
  //      root> NeutronAna t
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
  // by  b_branchname->GetEntry(ientry); //read only this branch
  int N = 200;
  TH1D *h = new TH1D(Form("h_%1.1f", fExcitationE), "", N, GetLogBin(N));

  if (fChain == 0)
    return h;

  Long64_t nentries = fChain->GetEntriesFast();

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry = 0; jentry < nentries; jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0)
      break;
    nb = fChain->GetEntry(jentry);
    nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    h->Fill(Energy);
  }

  return h;
}
