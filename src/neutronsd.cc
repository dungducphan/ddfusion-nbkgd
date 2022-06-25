#include "neutronsd.hh"

NeutronSD::NeutronSD(const G4String &name) : G4VSensitiveDetector(name) {}

NeutronSD::~NeutronSD() {}

G4bool NeutronSD::ProcessHits(G4Step *aStep, G4TouchableHistory *) {
    G4Track *track = aStep->GetTrack();

    if (track->GetDefinition() != G4ParticleTable::GetParticleTable()->FindParticle("neutron")) {
        track->SetTrackStatus(fStopAndKill);
        return false;
    }

    G4StepPoint *prePoint = aStep->GetPreStepPoint();
    auto processName = track->GetCreatorProcess()->GetProcessName();
    auto volumeName = track->GetOriginTouchable()->GetVolume()->GetName();

    int SensitiveDetectorID = 0;
    if (prePoint->GetPhysicalVolume()->GetName() == "physSD_000") {
        SensitiveDetectorID = 0;
    } else if (prePoint->GetPhysicalVolume()->GetName() == "physSD_001") {
        SensitiveDetectorID = 1;
    } else if (prePoint->GetPhysicalVolume()->GetName() == "physSD_002") {
        SensitiveDetectorID = 2;
    } else if (prePoint->GetPhysicalVolume()->GetName() == "physSD_003") {
        SensitiveDetectorID = 3;
    } else if (prePoint->GetPhysicalVolume()->GetName() == "physSD_004") {
        SensitiveDetectorID = 4;
    } else if (prePoint->GetPhysicalVolume()->GetName() == "physSD_005") {
        SensitiveDetectorID = 5;
    } else if (prePoint->GetPhysicalVolume()->GetName() == "physSD_006") {
        SensitiveDetectorID = 6;
    } else {
        SensitiveDetectorID = -1;
    }

    G4int parentProcessID = 0;
    if (processName == "ionInelastic") {
        parentProcessID = volumeName == "physBeConverter" ? 1 : 2;
    } else if (processName == "neutronInelastic") {
        parentProcessID = volumeName == "physBeConverter" ? 3 : 4;
    } else {
        parentProcessID = 5;
    }

//  G4cout << "Neutron hit detID " << SensitiveDetectorID << "." << G4endl;

    G4double energy = prePoint->GetKineticEnergy();
    G4double x_hit = prePoint->GetPosition().getX();
    G4double y_hit = prePoint->GetPosition().getY();
    G4double z_hit = prePoint->GetPosition().getZ();
    G4double t_hit = prePoint->GetGlobalTime();

    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->FillNtupleDColumn(0, energy / MeV);
    man->FillNtupleDColumn(1, x_hit / m);
    man->FillNtupleDColumn(2, y_hit / m);
    man->FillNtupleDColumn(3, z_hit / m);
    man->FillNtupleDColumn(4, t_hit / ns);
    man->FillNtupleDColumn(5, parentProcessID);
    man->FillNtupleDColumn(6, SensitiveDetectorID);
    man->AddNtupleRow(0);

    track->SetTrackStatus(fStopAndKill);
    return true;
}
