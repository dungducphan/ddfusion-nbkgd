#pragma once

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4ThreeVector.hh"
#include "G4Event.hh"
#include "G4String.hh"

class MyPrimaryGenerator : public G4VUserPrimaryGeneratorAction {
public:
  MyPrimaryGenerator();
  ~MyPrimaryGenerator() override;

  void GeneratePrimaries(G4Event* anEvent) override;

private:
  G4ParticleGun *fParticleGun = nullptr;
};
