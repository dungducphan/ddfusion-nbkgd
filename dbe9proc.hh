//
// Created by dphan on 5/2/22.
//

#ifndef DDFUSION_NBKGD_DBE9PROC_HH
#define DDFUSION_NBKGD_DBE9PROC_HH


#include "G4VProcess.hh"

class dbe9proc : public G4VProcess  {
public:
    //  Constructors
    explicit dbe9proc(const G4String& processName = "DeuteriumBreakUp");

    //  Destructor
    virtual ~dbe9proc();

private:
    //  copy constructor
    dbe9proc(const dbe9proc &right);

    //  Assignment Operation (generated)
    dbe9proc & operator=(const dbe9proc &right);


public: // With Description
    // G4Decay Process has both
    // PostStepDoIt (for decay in flight)
    //   and
    // AtRestDoIt (for decay at rest)

    virtual G4VParticleChange *PostStepDoIt(
            const G4Track& aTrack,
            const G4Step& aStep
    ) override;

    virtual G4VParticleChange* AtRestDoIt(
            const G4Track& aTrack,
            const G4Step&  aStep
    ) override;

    virtual void BuildPhysicsTable(const G4ParticleDefinition&) override;
    // In G4Decay, thePhysicsTable stores values of
    //    beta * std::sqrt( 1 - beta*beta)
    //  as a function of normalized kinetic enregy (=Ekin/mass),
    //  becasuse this table is universal for all particle types,


    virtual G4bool IsApplicable(const G4ParticleDefinition&) override;
    // returns "true" if the decay process can be applied to
    // the particle type.
};


#endif //DDFUSION_NBKGD_DBE9PROC_HH
