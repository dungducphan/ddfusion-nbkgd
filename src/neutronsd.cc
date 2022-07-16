#include "neutronsd.hh"

NeutronSD::NeutronSD(const G4String &name) : G4VSensitiveDetector(name) {}

NeutronSD::~NeutronSD() {}

G4bool NeutronSD::ProcessHits(G4Step *aStep, G4TouchableHistory *) {
    G4Track *track = aStep->GetTrack();

    if (track->GetDefinition() != G4ParticleTable::GetParticleTable()->FindParticle("neutron")) {
        track->SetTrackStatus(fStopAndKill);
        return false;
    }

    G4String processName;
    G4String volumeName;
    G4StepPoint *prePoint;
    if (track->GetParentID() != 0) {
        prePoint = aStep->GetPreStepPoint();
        processName = track->GetCreatorProcess()->GetProcessName();
        volumeName = track->GetOriginTouchable()->GetVolume()->GetName();
    } else {
        processName = "fusion";
        volumeName = "sourceVolume";
    }

    int SensitiveDetectorID = 0;
    if (track->GetParentID() != 0) {
        SensitiveDetectorID = prePoint->GetPhysicalVolume()->GetCopyNo();
    } else {
        SensitiveDetectorID = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetCopyNo();
    }

    G4int parentProcessID = 0;
    if (processName == "ionInelastic") {
        parentProcessID = volumeName == "physBeConverter" ? 1 : 2;
    } else if (processName == "neutronInelastic") {
        parentProcessID = volumeName == "physBeConverter" ? 3 : 4;
    } else if (processName == "fusion") {
        parentProcessID = 0;
    } else {
        parentProcessID = 5;
    }

    G4double energy;
    G4double x_hit;
    G4double y_hit;
    G4double z_hit;
    G4double t_hit;
    if (track->GetParentID() != 0) {
        energy = prePoint->GetKineticEnergy();
        x_hit = prePoint->GetPosition().getX();
        y_hit = prePoint->GetPosition().getY();
        z_hit = prePoint->GetPosition().getZ();
        t_hit = prePoint->GetGlobalTime();
    } else {
        energy = aStep->GetPreStepPoint()->GetKineticEnergy();
        x_hit = aStep->GetPreStepPoint()->GetPosition().getX();
        y_hit = aStep->GetPreStepPoint()->GetPosition().getY();
        z_hit = aStep->GetPreStepPoint()->GetPosition().getZ();
        t_hit = aStep->GetPreStepPoint()->GetGlobalTime();
    }

    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->FillNtupleDColumn(0,0, energy / MeV);
    man->FillNtupleDColumn(0,1, x_hit / m);
    man->FillNtupleDColumn(0,2, y_hit / m);
    man->FillNtupleDColumn(0,3, z_hit / m);
    man->FillNtupleDColumn(0,4, t_hit / ns);
    man->FillNtupleDColumn(0,5, parentProcessID);
    man->FillNtupleDColumn(0,6, SensitiveDetectorID);
    man->AddNtupleRow(0);

    track->SetTrackStatus(fStopAndKill);

    return true;
}
