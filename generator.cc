#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator() {
  fGeneralParticleSource = new G4GeneralParticleSource();
  fGeneralParticleSource->AddaSource(1.0);
  
  // First source: excited deuterium nuclei
  // ------------------------------------------------------------------------------------------------------------------------
  G4SingleParticleSource* exDeuteriumIon = fGeneralParticleSource->GetCurrentSource();

  exDeuteriumIon->SetParticleDefinition(G4IonTable::GetIonTable()->GetIon(1, 2, 2 * MeV));
  
  exDeuteriumIon->GetPosDist()->SetPosDisType("Volume"); // Point, Beam, Plane, Surface, Volume
  exDeuteriumIon->GetPosDist()->SetPosDisShape("Sphere"); // Square, Circle, Ellipse, Rectangle, Sphere, Ellipsoid, Cylinder, Parallelepiped
  exDeuteriumIon->GetPosDist()->SetRadius(2*um);
  exDeuteriumIon->GetPosDist()->SetCentreCoords(G4ThreeVector(0., 0., 4. * cm));
  exDeuteriumIon->GetPosDist()->SetPosRot1(G4ThreeVector(0., 0., 0.));
  exDeuteriumIon->GetPosDist()->SetPosRot2(G4ThreeVector(0., 0., 0.));
  exDeuteriumIon->GetPosDist()->ConfineSourceToVolume("NULL");

  exDeuteriumIon->GetAngDist()->SetAngDistType("Isotropic"); // Isotropic, Cosine-law, Beam, User-defined
  exDeuteriumIon->GetAngDist()->DefineAngRefAxes("angref1", G4ThreeVector(1, 0, 0));
  exDeuteriumIon->GetAngDist()->DefineAngRefAxes("angref2", G4ThreeVector(1, 1, 0));
  exDeuteriumIon->GetAngDist()->SetMinTheta(0.);
  exDeuteriumIon->GetAngDist()->SetMaxTheta(TMath::Pi() * rad);
  exDeuteriumIon->GetAngDist()->SetMinPhi(0.);
  exDeuteriumIon->GetAngDist()->SetMaxPhi(2 * TMath::Pi() * rad);

  // ------------------------------------------------------------------------------------------------------------------------

}

MyPrimaryGenerator::~MyPrimaryGenerator() {
  delete fGeneralParticleSource;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event* anEvent) {


  fGeneralParticleSource->GeneratePrimaryVertex(anEvent);
}
