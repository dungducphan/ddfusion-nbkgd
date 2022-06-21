#include "runaction.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "TString.h"

MyRunAction::MyRunAction() : G4UserRunAction() {
    man = G4AnalysisManager::Instance();
    man->CreateNtuple("Neutron", "Neutron");
    man->CreateNtupleDColumn("Energy");
    man->CreateNtupleDColumn("X");
    man->CreateNtupleDColumn("Y");
    man->CreateNtupleDColumn("Z");
    man->CreateNtupleDColumn("T");
    man->CreateNtupleDColumn("procID");
    man->CreateNtupleDColumn("detID");
    man->FinishNtuple(0);
}

MyRunAction::~MyRunAction() {
}

void MyRunAction::BeginOfRunAction(const G4Run* run) {
  man = G4AnalysisManager::Instance();
  man->OpenFile(Form("neutron_run%05i.root", (int) run->GetRunID()));

}

void MyRunAction::EndOfRunAction(const G4Run*) {
  man = G4AnalysisManager::Instance();
  man->Write();
  man->CloseFile();
}

