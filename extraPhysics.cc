//
// Created by dphan on 5/2/22.
//

#include "extraPhysics.hh.bkup"
#include "dbe9proc.hh.bkup"

#include "G4IonConstructor.hh"
#include "G4BuilderType.hh"

extraPhysics::extraPhysics(G4int ver) :  G4VPhysicsConstructor("DeuteriumBreakUp"), verbose(ver) {
    SetPhysicsType(bHadronInelastic);
}

extraPhysics::extraPhysics(const G4String &name, G4int ver) : G4VPhysicsConstructor(name), verbose(ver) {
    SetPhysicsType(bHadronInelastic);
}

extraPhysics::~extraPhysics() {
}

void extraPhysics::ConstructParticle() {
    G4IonConstructor pIonConstructor;
    pIonConstructor.ConstructParticle();
}

void extraPhysics::ConstructProcess() {
    G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();

    // Add Decay Process
//    dbe9proc* proc = new dbe9proc();
//    auto myParticleIterator = GetParticleIterator();
//    myParticleIterator->reset();
//    G4ParticleDefinition* particle = 0;
//
//    while((*myParticleIterator)()) {
//        particle = myParticleIterator->value();
//        if(proc->IsApplicable(*particle)) {
//            if(verbose > 1) {
//                G4cout << "### Inelastic scattering of" << particle->GetParticleName() << " on Be-9." << G4endl;
//            }
//            ph->RegisterProcess(proc, particle);
//        }
//    }
}
