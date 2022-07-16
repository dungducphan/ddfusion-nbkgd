#include "deuteronsd.hh"

DeuteronSD::DeuteronSD(const G4String &name) : G4VSensitiveDetector(name) {}

DeuteronSD::~DeuteronSD() {}

G4bool DeuteronSD::ProcessHits(G4Step *aStep, G4TouchableHistory *) {
    if (aStep->GetTrack()->GetDefinition() != G4ParticleTable::GetParticleTable()->GetIonTable()
    ->GetIon(1, 2)) {
        return false;
    }

    int SensitiveDetectorID = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetCopyNo();

    G4double energy;
    G4double x_hit;
    G4double y_hit;
    G4double z_hit;
    G4double t_hit;
    energy = aStep->GetPreStepPoint()->GetKineticEnergy();
    x_hit = aStep->GetPreStepPoint()->GetPosition().getX();
    y_hit = aStep->GetPreStepPoint()->GetPosition().getY();
    z_hit = aStep->GetPreStepPoint()->GetPosition().getZ();
    t_hit = aStep->GetPreStepPoint()->GetGlobalTime();

    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->FillNtupleDColumn(1,0, energy / MeV);
    man->FillNtupleDColumn(1,1, x_hit / m);
    man->FillNtupleDColumn(1,2, y_hit / m);
    man->FillNtupleDColumn(1,3, z_hit / m);
    man->FillNtupleDColumn(1,4, t_hit / ns);
    man->FillNtupleDColumn(1,5, SensitiveDetectorID);
    man->AddNtupleRow(1);

    return true;
}
