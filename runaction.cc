#include "runaction.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"

MyRunAction::MyRunAction() : G4UserRunAction() {
}

MyRunAction::~MyRunAction() {
}

void MyRunAction::BeginOfRunAction(const G4Run*) {
  G4AnalysisManager *man = G4AnalysisManager::Instance();
  man->OpenFile("deuterium.root");

  man->CreateNtuple("Primary", "Primary");
  man->CreateNtupleDColumn("Energy");
  man->FinishNtuple(0);
}

void MyRunAction::EndOfRunAction(const G4Run*) {
  G4AnalysisManager *man = G4AnalysisManager::Instance();
  man->Write();
  man->CloseFile();
}

