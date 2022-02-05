#include "runaction.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "TString.h"

MyRunAction::MyRunAction() : G4UserRunAction() {
}

MyRunAction::~MyRunAction() {
}

void MyRunAction::BeginOfRunAction(const G4Run* run) {
  G4AnalysisManager *man = G4AnalysisManager::Instance();
  man->OpenFile(Form("neutron_run%05i.root", (int) run->GetRunID()));

  man->CreateNtuple("Neutron", "Neutron");
  man->CreateNtupleDColumn("Energy");
  man->FinishNtuple((int) run->GetRunID());
}

void MyRunAction::EndOfRunAction(const G4Run*) {
  G4AnalysisManager *man = G4AnalysisManager::Instance();
  man->Write();
  man->CloseFile();
}

