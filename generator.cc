#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator() {
  fGeneralParticleSource = new G4GeneralParticleSource();
}

MyPrimaryGenerator::~MyPrimaryGenerator() {
  delete fGeneralParticleSource;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event* anEvent) {

  // What particle is that?
  G4ParticleDefinition *exDeuteriumIon = G4IonTable::GetIonTable()->GetIon(1, 2, 2 * MeV);
  fGeneralParticleSource->SetParticleDefinition(exDeuteriumIon);

  // What is spatial distribution of the source?
  G4SPSPosDistribution* spatialDist = new G4SPSPosDistribution();
  spatialDist->SetPosDisType("Volume"); // Point, Beam, Plane, Surface, Volume
  spatialDist->SetPosDisShape("Sphere"); // Square, Circle, Ellipse, Rectangle, Sphere, Ellipsoid, Cylinder, Parallelepiped
  spatialDist->SetRadius(2*um);
  spatialDist->SetCentreCoords(G4ThreeVector(0., 0., 0.));


  fGeneralParticleSource->GeneratePrimaryVertex(anEvent);
}
