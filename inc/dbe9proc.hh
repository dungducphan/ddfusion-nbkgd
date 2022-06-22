//
// Created by dphan on 5/2/22.
//

#ifndef DDFUSION_NBKGD_DBE9PROC_HH
#define DDFUSION_NBKGD_DBE9PROC_HH


#include "G4VDiscreteProcess.hh"
#include "G4Deuteron.hh"
#include "G4SystemOfUnits.hh"
#include "G4Neutron.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4Proton.hh"
#include "G4ParticleDefinition.hh"
#include "TMath.h"
#include "TRandom3.h"
#include "TLorentzVector.h"

class dbe9proc : public G4VDiscreteProcess  {
public:
    //  Constructors
    explicit dbe9proc(G4double energyTransfer, const G4String& processName = "DeuteriumBreakUp");

    //  Destructor
    ~dbe9proc() override;

private:
    //  copy constructor
    dbe9proc(const dbe9proc &right);

    //  Assignment Operation (generated)
    dbe9proc & operator=(const dbe9proc &right);

    G4double fCurrentSigma;    // the last value of cross-section per volume
    G4double fEnergyTransfer;
    TRandom3* myRndGen;

public: // With Description

    [[maybe_unused]] void SetEnergyTransfer(G4double eTransfer);
    [[maybe_unused]] [[nodiscard]] G4double GetEnergyTransfer() const;

    G4VParticleChange *PostStepDoIt(
            const G4Track& aTrack,
            const G4Step& aStep
    ) override;

    void BuildPhysicsTable(const G4ParticleDefinition&) override;

    static G4double ComputeCrossSectionPerDeuteronBe9Pair(G4double energyInMeV);
    static G4double CrossSectionPerVolume(G4double energy, const G4Material* aMaterial);
    G4double GetMeanFreePath(const G4Track& aTrack,
                             G4double previousStepSize,
                             G4ForceCondition* ) override;

    static G4ThreeVector NeutronUnitMomentumSolver(const G4Track& aDeuteronTrack);
    G4double NeutronKineticEnergySolver(const G4Track& aDeuteronTrack, const G4ThreeVector& aNeutronUnitMomentum);
    G4double NeutronKineticEnergySolverSimple(const G4Track& aDeuteronTrack);

    virtual G4bool IsApplicable(const G4ParticleDefinition&) override;
};


#endif //DDFUSION_NBKGD_DBE9PROC_HH
