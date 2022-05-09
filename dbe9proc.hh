//
// Created by dphan on 5/2/22.
//

#ifndef DDFUSION_NBKGD_DBE9PROC_HH
#define DDFUSION_NBKGD_DBE9PROC_HH


#include "G4VDiscreteProcess.hh"
#include "G4Deuteron.hh"
#include "G4SystemOfUnits.hh"
#include "G4Neutron.hh"
#include "TMath.h"

class dbe9proc : public G4VDiscreteProcess  {
public:
    //  Constructors
    explicit dbe9proc(const G4String& processName = "DeuteriumBreakUp");

    //  Destructor
    ~dbe9proc() override;

private:
    //  copy constructor
    dbe9proc(const dbe9proc &right);

    //  Assignment Operation (generated)
    dbe9proc & operator=(const dbe9proc &right);

    G4double fCurrentSigma;    // the last value of cross section per volume


public: // With Description

    virtual G4VParticleChange *PostStepDoIt(
            const G4Track& aTrack,
            const G4Step& aStep
    ) override;

    virtual void BuildPhysicsTable(const G4ParticleDefinition&) override;
    // dummy

    G4double ComputeCrossSectionPerDeuteronBe9Pair(const G4double energyInMeV);
    G4double CrossSectionPerVolume(G4double energy, const G4Material* aMaterial);
    G4double GetMeanFreePath(const G4Track& aTrack,
                             G4double previousStepSize,
                             G4ForceCondition* ) override;

    virtual G4bool IsApplicable(const G4ParticleDefinition&) override;
    // returns "true" if the particle involved is deuteron
};


#endif //DDFUSION_NBKGD_DBE9PROC_HH
