#define NeutronAna_cxx
#include "NeutronAna.h"
#include <TCanvas.h>
#include <TH2.h>
#include <TStyle.h>

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
