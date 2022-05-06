//
// Created by dphan on 5/2/22.
//

#ifndef DDFUSION_NBKGD_EXTRAPHYSICS_HH_BKUP
#define DDFUSION_NBKGD_EXTRAPHYSICS_HH_BKUP

#include "globals.hh"
#include "G4VPhysicsConstructor.hh"

class extraPhysics : public G4VPhysicsConstructor
{
public:
    explicit extraPhysics(G4int ver = 1);
    explicit extraPhysics(const G4String& name, G4int ver = 1);
    ~extraPhysics() override;

    // This method will be invoked in the Construct() method.
    // each particle type will be instantiated
    void ConstructParticle() override;

    // This method will be invoked in the Construct() method.
    // each physics process will be instantiated and
    // registered to the process manager of each particle type
    void ConstructProcess() override;

private:
    G4int    verbose;
};

#endif //DDFUSION_NBKGD_EXTRAPHYSICS_HH_BKUP
