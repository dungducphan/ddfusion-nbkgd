//
// Created by dphan on 5/2/22.
//

#include "dbe9proc.hh"

dbe9proc::dbe9proc(const G4String &processName) : G4VDiscreteProcess(processName) {
    SetProcessType(fHadronic);
    SetProcessSubType(121);
}

G4bool dbe9proc::IsApplicable(const G4ParticleDefinition &aParticleType) {
    // Apply this model to deuteron only
    return &aParticleType == G4Deuteron::Deuteron();

}

void dbe9proc::BuildPhysicsTable(const G4ParticleDefinition &) {
    return;
}

dbe9proc::~dbe9proc() {
}

G4VParticleChange *dbe9proc::PostStepDoIt(const G4Track &aTrack, const G4Step &aStep) {
    aParticleChange.Initialize(aTrack);

    // current positron energy and direction, return if energy too low
    const G4DynamicParticle *aDynamicDeuterium = aTrack.GetDynamicParticle();
    // FIXME: is this supposed to be total energy or kinetic energy?
    G4double Edeu = aDynamicDeuterium->GetKineticEnergy();
    G4cout << Edeu / MeV << G4endl;
    G4double totXs = CrossSectionPerVolume(Edeu, aTrack.GetMaterial());

    // test of cross-section
    if (totXs > 0.0 && fCurrentSigma*G4UniformRand() > totXs) {
        return G4VDiscreteProcess::PostStepDoIt(aTrack,aStep); // just reset NumberOfInteractionLengthLeft
    }

    // care only about neutron, don't worry about X or the break-up proton
    aParticleChange.SetNumberOfSecondaries(1);

    // Solve for neutron unit momentum and kinetic energy (in lab frame, of course)
    G4ThreeVector Neutron_LF_UnitMomentum = NeutronUnitMomentumSolver(aTrack);
    // TODO:
    G4double Neutron_LF_KEnergy = 0;

    // create G4DynamicParticle object for the generated neutron
    G4DynamicParticle* aNeutron = new G4DynamicParticle(G4Neutron::Definition(), Neutron_LF_UnitMomentum, Neutron_LF_KEnergy);
    aParticleChange.AddSecondary(aNeutron);

    // Kill the incident deuteron
    aParticleChange.ProposeEnergy(0.);
    aParticleChange.ProposeTrackStatus(fStopAndKill);

    return &aParticleChange;
}

G4double dbe9proc::ComputeCrossSectionPerDeuteronBe9Pair(const G4double energyInMeV) {
    G4double x = energyInMeV/MeV;

    // Apply cut to avoid unphysical xsec values
    if (x < 0.7 || x > 12.8) {
        return 0;
    } else {
        G4double sig =
                -0.426331 + 0.862892 * x - 0.297566 * x * x + 0.0524488 * x * x * x - 0.00422005 * x * x * x * x +
                0.000120617 * x * x * x * x * x;
        return sig * barn;
    }
}

G4double dbe9proc::CrossSectionPerVolume(G4double energyInMeV, const G4Material *aMaterial) {
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

G4ThreeVector dbe9proc::NeutronUnitMomentumSolver(const G4Track &aTrack) {
    G4LorentzVector P4deu = aTrack.GetDynamicParticle()->Get4Momentum();

    // First, pick a random unit vector in CoM frame for neutron momentum
    G4double theta = TMath::Pi()*G4UniformRand();
    G4double phi = TMath::TwoPi()*G4UniformRand();
    G4double energy = TMath::Sqrt(TMath::Power(G4Neutron::Definition()->GetPDGMass(), 2) + 100);
    TLorentzVector Neutron_CM;
    Neutron_CM.SetPxPyPzE(TMath::Sin(theta)*TMath::Cos(phi), TMath::Sin(theta)*TMath::Sin(phi), TMath::Cos(theta), energy * MeV);

    // Calculate the boost vector
    TLorentzVector Deuteron_LF;
    G4double deuteronMomentum = 5;
    Deuteron_LF.SetPxPyPzE(P4deu.px(), P4deu.py(), P4deu.pz(), P4deu.e());
    TVector3 BoostingVector = Deuteron_LF.BoostVector();

    // Apply boost to find neutron unit momentum in the lab frame
    TLorentzVector Neutron_LF = Neutron_CM;
    Neutron_LF.Boost(-BoostingVector);

    return G4ThreeVector(Neutron_LF.Px() / Neutron_LF.P(), Neutron_LF.Py() / Neutron_LF.P(), Neutron_LF.Pz() / Neutron_LF.P());
}

G4double dbe9proc::NeutronKineticEnergySolver(const G4Track &aDeuteronTrack, const G4ThreeVector &aNeutronUnitMomentum) {
    // TODO: Implement the energy solver here
    return 0;
}
