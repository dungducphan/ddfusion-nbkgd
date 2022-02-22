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
  G4cout << "Neutron produced by: " << track->GetCreatorProcess()->GetProcessName() << " in " << track->GetOriginTouchable()->GetVolume()->GetName() << G4endl;
  auto processName = track->GetCreatorProcess()->GetProcessName();
  auto volumeName = track->GetOriginTouchable()->GetVolume()->GetName();
  G4int parentProcessID = 0;
  if (processName == "ionInelastic") {
    parentProcessID = 1;
  } else if (processName == "neutronInelastic") {
    parentProcessID = volumeName == "physBeConverter" ? 2 : 3;
  } else {
    parentProcessID = 4;
  }

  G4double energy = prePoint->GetKineticEnergy();
  G4double x_hit = prePoint->GetPosition().getX();
  G4double y_hit = prePoint->GetPosition().getY();
  G4double z_hit = prePoint->GetPosition().getZ();
  G4double t_hit = prePoint->GetGlobalTime();

  G4AnalysisManager* man = G4AnalysisManager::Instance();
  man->FillNtupleDColumn(0, energy / MeV);
  man->FillNtupleDColumn(1, x_hit / m);
  man->FillNtupleDColumn(2, y_hit / m);
  man->FillNtupleDColumn(3, z_hit / m);
  man->FillNtupleDColumn(4, t_hit / ns);
  man->FillNtupleDColumn(5, parentProcessID);
  man->AddNtupleRow(0);

  track->SetTrackStatus(fStopAndKill);
  return true;
}
