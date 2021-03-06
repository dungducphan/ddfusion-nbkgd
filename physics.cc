#include "physics.hh"
#include "extraPhysics.hh"

MyPhysicsList::MyPhysicsList() : G4VModularPhysicsList() {
    G4int verb = 0;
    SetVerboseLevel(verb);

    // Mandatory for G4NuclideTable
    // Half-life threshold must be set small or many short-lived isomers
    // will not be assigned life times (default to 0)
    G4NuclideTable::GetInstance()->SetThresholdOfHalfLife(1 * picosecond);
    G4NuclideTable::GetInstance()->SetLevelTolerance(1 * eV);

    // EM physics
    RegisterPhysics(new G4EmStandardPhysics());
    G4EmParameters *param = G4EmParameters::Instance();
    param->SetAugerCascade(true);
    param->SetStepFunction(1., 1 * CLHEP::mm);
    param->SetStepFunctionMuHad(1., 1 * CLHEP::mm);

    // Decay
    RegisterPhysics(new G4DecayPhysics());

    // Hadron Elastic scattering
    RegisterPhysics(new G4HadronElasticPhysics(verb));

    // Hadron Inelastic physics
    // Using High-Precision Neutron Transport Model
    RegisterPhysics(new G4HadronPhysicsINCLXX("INCLHADINELAS", true, true, false));

    // Ion Elastic scattering
    RegisterPhysics(new G4IonElasticPhysics(verb));

    // Ion Inelastic physics
/*
  // Ignore this user-defined physics because its contribution to neutron yield is proved to be very small.
  // RegisterPhysics( new extraPhysics("extraPhysics", 3.0 * MeV, 3));
*/
    RegisterPhysics(new G4IonINCLXXPhysics(verb));


    // Gamma-Nuclear Physics
    G4EmExtraPhysics *gnuc = new G4EmExtraPhysics(verb);
    gnuc->ElectroNuclear(true);
    gnuc->MuonNuclear(true);
    RegisterPhysics(gnuc);
}

MyPhysicsList::~MyPhysicsList() {}

void MyPhysicsList::ConstructParticle() {
    G4BosonConstructor pBosonConstructor;
    pBosonConstructor.ConstructParticle();

    G4LeptonConstructor pLeptonConstructor;
    pLeptonConstructor.ConstructParticle();

    G4MesonConstructor pMesonConstructor;
    pMesonConstructor.ConstructParticle();

    G4BaryonConstructor pBaryonConstructor;
    pBaryonConstructor.ConstructParticle();

    G4IonConstructor pIonConstructor;
    pIonConstructor.ConstructParticle();

    G4ShortLivedConstructor pShortLivedConstructor;
    pShortLivedConstructor.ConstructParticle();
}

void MyPhysicsList::SetCuts() {
    SetCutValue(0 * mm, "proton");
    SetCutValue(10 * km, "e-");
    SetCutValue(10 * km, "e+");
    SetCutValue(10 * km, "gamma");
}

