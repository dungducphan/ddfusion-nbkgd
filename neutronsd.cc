#include "neutronsd.hh"

NeutronSD::NeutronSD(const G4String &name) : G4VSensitiveDetector(name) {}

NeutronSD::~NeutronSD() {}

G4bool NeutronSD::ProcessHits(G4Step * aStep, G4TouchableHistory *) {
  G4Track *track = aStep->GetTrack();

  if (track->GetDefinition() != G4ParticleTable::GetParticleTable()->FindParticle("neutron")) {
    track->SetTrackStatus(fStopAndKill);
    return false;
  }

  G4StepPoint *prePoint = aStep->GetPreStepPoint();

  G4double energy = prePoint->GetKineticEnergy();
  G4double x_hit = prePoint->GetPosition().getX();
  G4double y_hit = prePoint->GetPosition().getY();
  G4double z_hit = prePoint->GetPosition().getZ();

  G4AnalysisManager* man = G4AnalysisManager::Instance();
  man->FillNtupleDColumn(0, energy / MeV);
  man->FillNtupleDColumn(1, x_hit / m);
  man->FillNtupleDColumn(2, y_hit / m);
  man->FillNtupleDColumn(3, z_hit / m);
  man->AddNtupleRow(0);

  track->SetTrackStatus(fStopAndKill);
  return true;
}
