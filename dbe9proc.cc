//
// Created by dphan on 5/2/22.
//

#include "dbe9proc.hh"

dbe9proc::dbe9proc(const G4String &processName) : G4VDiscreteProcess(processName) {
    SetProcessType(fHadronic);
    SetProcessSubType(121);
}

G4bool dbe9proc::IsApplicable(const G4ParticleDefinition &aParticleType) {
    // FIXME: check if the particle is Deuteron?
    return &aParticleType == G4Deuteron::Deuteron();

}

void dbe9proc::BuildPhysicsTable(const G4ParticleDefinition &) {
    return;
}

dbe9proc::~dbe9proc() {
}

G4VParticleChange *dbe9proc::PostStepDoIt(const G4Track &aTrack, const G4Step &aStep) {
    aParticleChange.Initialize(aTrack);

    // current Positron energy and direction, return if energy too low
    const G4DynamicParticle *aDynamicDeuterium = aTrack.GetDynamicParticle();
    G4double Edeu = aDynamicDeuterium->GetTotalEnergy();
    G4double totXs = CrossSectionPerVolume(Edeu, aTrack.GetMaterial());

    // test of cross section
    if (totXs > 0.0 && fCurrentSigma*G4UniformRand() > totXs) {
        return G4VDiscreteProcess::PostStepDoIt(aTrack,aStep); // just reset NumberOfInteractionLengthLeft
    }

    // interaction descriptions

    // care only about neutron, don't worry about X or the break-up proton
    aParticleChange.SetNumberOfSecondaries(1);

    // TODO: generate fake energy spectrum, redo calculations with Ou/Lance and implement the calculation here
    // CALCULATION START
    // 1. G4ThreeVector MuPlusDirection.
    // 2. G4double
    // CALCULATION END

    // FIXME: fake calculations
    G4ParticleDefinition* genNeutronDef = G4Neutron::Definition();
    G4double NeutronEnergy = 5 * MeV;
    // FIXME: Isotropic neutron momentum, maybe good enough, need not to change?
    G4double theta = TMath::Pi()*G4UniformRand();
    G4double phi = TMath::TwoPi()*G4UniformRand();
    G4ThreeVector NeutronDirection(TMath::Sin(theta)*TMath::Cos(phi), TMath::Sin(theta)*TMath::Sin(phi), TMath::Cos(theta));
    // FIXME: END

    // create G4DynamicParticle object for the generated neutron
    G4DynamicParticle* aNeutron = new G4DynamicParticle(genNeutronDef, NeutronDirection, NeutronEnergy);
    aParticleChange.AddSecondary(aNeutron);

    // Kill the incident deuteron
    aParticleChange.ProposeEnergy(0.);
    aParticleChange.ProposeTrackStatus(fStopAndKill);

    return &aParticleChange;
}

G4double dbe9proc::ComputeCrossSectionPerDeuteronBe9Pair(const G4double energyInMeV) {
    G4double x = energyInMeV/MeV;

//    // FIXME: faking a delta function cross-section to make sure that the code work first
//    if (x < 10 || x > 15) {
//        return 0 * barn;
//    } else {
//    // FIXME: Testing. Remove once done!
//        // G4cout << "Calculating the cross-section for 'new' physics." << G4endl;
//        return 10 * barn;
//    }

    // TODO: this function below is the cross-section in term of E_cm from Ou, use it for production
    G4double sig = -0.426331 + 0.862892*x - 0.297566*x*x + 0.0524488*x*x*x - 0.00422005*x*x*x*x + 0.000120617*x*x*x*x*x;
    return sig * barn;
}

G4double dbe9proc::CrossSectionPerVolume(G4double energyInMeV, const G4Material *aMaterial) {
//    FIXME: Apply the D-Be9 for any material, which is wrong. For testing purpose only.
//    return ComputeCrossSectionPerDeuteronBe9Pair(energyInMeV) * aMaterial->GetTotNbOfAtomsPerVolume();
//    TODO: check here as well, ideally we just want D interacts with Be9. Also, need to scale the
//          x-sec by the nuclei density of the material.
    if (aMaterial->GetName() != "G4_Be") return 0;
    else {
        // G4cout << "Calculating the cross-section for 'new' physics." << G4endl;
        return ComputeCrossSectionPerDeuteronBe9Pair(energyInMeV) * aMaterial->GetTotNbOfAtomsPerVolume();
    }
}

G4double dbe9proc::GetMeanFreePath(const G4Track &aTrack, G4double previousStepSize, G4ForceCondition *) {
    const G4DynamicParticle* aDynamicDeuteron = aTrack.GetDynamicParticle();
    G4double energy = aDynamicDeuteron->GetTotalEnergy();
    const G4Material* aMaterial = aTrack.GetMaterial();

    // cross-section before step
    fCurrentSigma = CrossSectionPerVolume(energy, aMaterial);

    // increase the CrossSection by CrossSecFactor (default 1)
    return (fCurrentSigma > 0.0) ? 1.0/fCurrentSigma : DBL_MAX;
}
