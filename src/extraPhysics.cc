//
// Created by dphan on 5/2/22.
//

#include "extraPhysics.hh"
#include "dbe9proc.hh"

#include "G4IonConstructor.hh"
#include "G4BuilderType.hh"

extraPhysics::extraPhysics(G4int ver) :  G4VPhysicsConstructor("DeuteriumBreakUp"), verbose(ver) {
    SetPhysicsType(bUnknown);
}

extraPhysics::extraPhysics(const G4String &name, G4int ver) : G4VPhysicsConstructor(name), verbose(ver) {
    SetPhysicsType(bUnknown);
}

extraPhysics::extraPhysics(const G4String &name, G4double eTransfer,  G4int ver) : G4VPhysicsConstructor(name), fEnergyTransfer(eTransfer), verbose(ver) {
    SetPhysicsType(bUnknown);
}

extraPhysics::~extraPhysics() {
}

void extraPhysics::ConstructParticle() {
    G4IonConstructor pIonConstructor;
    pIonConstructor.ConstructParticle();
}

void extraPhysics::ConstructProcess() {
    G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();

    // Add D-Be9 process
    dbe9proc* proc = new dbe9proc(fEnergyTransfer);
    auto myParticleIterator = GetParticleIterator();
    myParticleIterator->reset();
    G4ParticleDefinition* particle = 0;

    while((*myParticleIterator)()) {
        particle = myParticleIterator->value();
        if(proc->IsApplicable(*particle)) {
            if(verbose > 1) {
                G4cout << "### Inelastic scattering of " << particle->GetParticleName() << " on Be-9." << G4endl;
            }
            ph->RegisterProcess(proc, particle);
        }
    }
}

void extraPhysics::SetEnergyTransfer(G4double eTransfer) {
    fEnergyTransfer = eTransfer;
}

G4double extraPhysics::GetEnergyTransfer() const {
    return fEnergyTransfer;
}


