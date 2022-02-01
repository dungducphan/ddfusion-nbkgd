#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator() {
  fGeneralParticleSource = new G4GeneralParticleSource();
  
  // First source: excited deuterium nuclei
  // ------------------------------------------------------------------------------------------------------------------------
  G4SingleParticleSource* exDeuteriumIon = new G4SingleParticleSource();
  exDeuteriumIon->SetParticleDefinition(G4IonTable::GetIonTable()->GetIon(1, 2, 2 * MeV));
  exDeuteriumIon->GetPosDist()->SetPosDisType("Volume"); // Point, Beam, Plane, Surface, Volume
  exDeuteriumIon->GetPosDist()->SetPosDisShape("Sphere"); // Square, Circle, Ellipse, Rectangle, Sphere, Ellipsoid, Cylinder, Parallelepiped
  exDeuteriumIon->GetPosDist()->SetRadius(2*um);
  exDeuteriumIon->GetPosDist()->SetCentreCoords(G4ThreeVector(0., 0., 4. * cm));
  exDeuteriumIon->GetPosDist()->SetPosRot1(G4ThreeVector(0., 0., 0.));
  exDeuteriumIon->GetPosDist()->SetPosRot2(G4ThreeVector(0., 0., 0.));
  exDeuteriumIon->GetPosDist()->ConfineSourceToVolume("NULL");
  // ------------------------------------------------------------------------------------------------------------------------

  fGeneralParticleSource->SetParticleDefinition(exDeuteriumIon);
  fGeneralParticleSource->GetPos
}

MyPrimaryGenerator::~MyPrimaryGenerator() {
  delete fGeneralParticleSource;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event* anEvent) {


  fGeneralParticleSource->GeneratePrimaryVertex(anEvent);
}
