//
// Created by dphan on 5/2/22.
//

#include "dbe9proc.hh"

dbe9proc::dbe9proc(G4double energyTransfer, const G4String &processName) : G4VDiscreteProcess(processName), fCurrentSigma(0), fEnergyTransfer(energyTransfer) {
    SetProcessType(fHadronic);
    SetProcessSubType(121);
    myRndGen = new TRandom3();
    myRndGen->SetSeed(G4UniformRand()*1E6);
}

G4bool dbe9proc::IsApplicable(const G4ParticleDefinition &aParticleType) {
    // Apply this model to deuteron only
    return &aParticleType == G4Deuteron::Deuteron();

}

void dbe9proc::BuildPhysicsTable(const G4ParticleDefinition &) {}

dbe9proc::~dbe9proc() = default;

G4VParticleChange *dbe9proc::PostStepDoIt(const G4Track &aTrack, const G4Step &aStep) {
    aParticleChange.Initialize(aTrack);

    // current positron energy and direction, return if energy too low
    const G4DynamicParticle *aDynamicDeuterium = aTrack.GetDynamicParticle();
    G4double Edeu = aDynamicDeuterium->GetKineticEnergy();
    G4double totXs = CrossSectionPerVolume(Edeu, aTrack.GetMaterial());

    // test of cross-section
    if (totXs > 0.0 && fCurrentSigma*G4UniformRand() > totXs) {
        return G4VDiscreteProcess::PostStepDoIt(aTrack,aStep); // just reset NumberOfInteractionLengthLeft
    }

    // care only about neutron, don't worry about X or the break-up proton
//    G4cout << "Producing neutron." << G4endl;
    aParticleChange.SetNumberOfSecondaries(1);

    // Solve for neutron unit momentum and kinetic energy (in lab frame, of course)
    G4ThreeVector Neutron_LF_UnitMomentum = NeutronUnitMomentumSolver(aTrack);
    // FIXME:
//    G4double Neutron_LF_KEnergy = 100 * MeV;
//    G4double Neutron_LF_KEnergy = NeutronKineticEnergySolver(aTrack, Neutron_LF_UnitMomentum) + 50 * MeV;
    G4double Neutron_LF_KEnergy = NeutronKineticEnergySolverSimple(aTrack) + 50 * MeV;

    // create G4DynamicParticle object for the generated neutron
    auto* aNeutron = new G4DynamicParticle(G4Neutron::Definition(), Neutron_LF_UnitMomentum, Neutron_LF_KEnergy);
    aParticleChange.AddSecondary(aNeutron);

    // Kill the incident deuteron
    aParticleChange.ProposeEnergy(0.);
    aParticleChange.ProposeTrackStatus(fStopAndKill);

    return &aParticleChange;
}

G4double dbe9proc::ComputeCrossSectionPerDeuteronBe9Pair(G4double energyInMeV) {
    G4double x = energyInMeV/MeV;

    // Apply cut to avoid unphysical xsec values
    // Here the cross-section is scaled to 100. The signal is only visible when this scaling is applied.
    // Otherwise, it's negligible!
    if (x <= 0 || x >= 13) {
        return 0 * barn;
    } else {
        G4double sig =
                -0.426331 + 0.862892 * x - 0.297566 * x * x + 0.0524488 * x * x * x - 0.00422005 * x * x * x * x +
                0.000120617 * x * x * x * x * x;
        return sig > 0 ? sig * 100 * barn : 0;
    }
}

G4double dbe9proc::CrossSectionPerVolume(G4double energyInMeV, const G4Material *aMaterial) {
    // Apply this process on Be volume only
    if (aMaterial->GetName() != "G4_Be") return 0;
    else {
        return ComputeCrossSectionPerDeuteronBe9Pair(energyInMeV) * aMaterial->GetTotNbOfAtomsPerVolume();
    }
}

G4double dbe9proc::GetMeanFreePath(const G4Track &aTrack, G4double previousStepSize, G4ForceCondition *) {
    const G4DynamicParticle* aDynamicDeuteron = aTrack.GetDynamicParticle();
    G4double energy = aDynamicDeuteron->GetKineticEnergy();
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
    Deuteron_LF.SetPxPyPzE(P4deu.px(), P4deu.py(), P4deu.pz(), P4deu.e());
    TVector3 BoostingVector = Deuteron_LF.BoostVector();

    // Apply boost to find neutron unit momentum in the lab frame
    TLorentzVector Neutron_LF = Neutron_CM;
    Neutron_LF.Boost(-BoostingVector);

    return {Neutron_LF.Px() / Neutron_LF.P(), Neutron_LF.Py() / Neutron_LF.P(), Neutron_LF.Pz() / Neutron_LF.P()};
}

G4double dbe9proc::NeutronKineticEnergySolver(const G4Track &aDeuteronTrack, const G4ThreeVector &aNeutronUnitMomentum) {
    G4LorentzVector P4deu = aDeuteronTrack.GetDynamicParticle()->Get4Momentum();

    double cross_term = aNeutronUnitMomentum.x() * P4deu.x() +
                        aNeutronUnitMomentum.y() * P4deu.y() +
                        aNeutronUnitMomentum.z() * P4deu.z();
    double P3deu_squared =  P4deu.x() * P4deu.x() +
                            P4deu.y() * P4deu.y() +
                            P4deu.z() * P4deu.z();
    double MBe9 = G4ParticleTable::GetParticleTable()->GetIonTable()->GetIon(4, 9)->GetPDGMass();
    double MD = G4Deuteron::Definition()->GetPDGMass();
    double Mn = G4Neutron::Definition()->GetPDGMass();
    double Mp = G4Proton::Definition()->GetPDGMass();
    fEnergyTransfer = G4UniformRand() * Mp;
    double Mx = MD + MBe9 - Mn + fEnergyTransfer;

    // Solving a quadratic equation with variable is sqrt(E) with coefficients A, B, C.
    double A = 1;
    double B = - cross_term * TMath::Sqrt(2./Mn) / (Mx * (1 + Mn/Mx));
    double C = - (P3deu_squared / 2.) * (1/Mp - 1/Mx) + fEnergyTransfer;
    double Delta = B*B - 4*A*C;

    if (Delta < 0) {
        G4cout << " No sol: DEnergy = " << aDeuteronTrack.GetKineticEnergy() / MeV << ". fET = " << fEnergyTransfer << G4endl;
        return 0;
    }
    else if (Delta == 0) {
        double sol = -B / (2 * A);
        return sol > 0 ? sol : 0;
    } else {
        double sol1 = -B + TMath::Sqrt(Delta) / (2 * A);
        double sol2 = -B - TMath::Sqrt(Delta) / (2 * A);
        if (sol1 <= 0 && sol2 <= 0) return 0;
        else {
            if (sol1 <= 0) return sol2;
            else if (sol2 <= 0) return sol1;
            else {
                return G4UniformRand() < 0.5 ? sol1 : sol2;
            }
        }
    }
}

[[maybe_unused]] G4double dbe9proc::GetEnergyTransfer() const {
    return fEnergyTransfer;
}

[[maybe_unused]] void dbe9proc::SetEnergyTransfer(G4double eTransfer) {
    fEnergyTransfer = eTransfer;
}

G4double dbe9proc::NeutronKineticEnergySolverSimple(const G4Track &aDeuteronTrack) {
    G4double deuMom = aDeuteronTrack.GetDynamicParticle()->GetTotalMomentum();
    G4double nMomFraction = myRndGen->Gaus(50, 10) / 100.;

    return TMath::Power(deuMom * nMomFraction,2) / (2. * G4Neutron::Definition()->GetPDGMass());
}
