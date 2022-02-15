#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator() {
  fGeneralParticleSource = new G4GeneralParticleSource();

  // First source: excited deuterium nuclei
  // ------------------------------------------------------------------------------------------------------------------------
  G4SingleParticleSource *exDeuteriumIon =
      fGeneralParticleSource->GetCurrentSource();
  G4ParticleDefinition *exDIon =
      G4ParticleTable::GetParticleTable()->FindParticle("e-");

  exDeuteriumIon->SetParticleDefinition(exDIon);
  exDeuteriumIon->SetNumberOfParticles(100);

  exDeuteriumIon->GetPosDist()->SetPosDisType(
      "Volume"); // Point, Beam, Plane, Surface, Volume
  exDeuteriumIon->GetPosDist()->SetPosDisShape(
      "Sphere"); // Square, Circle, Ellipse, Rectangle, Sphere, Ellipsoid,
                 // Cylinder, Parallelepiped
  exDeuteriumIon->GetPosDist()->SetRadius(3 * um);
  exDeuteriumIon->GetPosDist()->SetCentreCoords(
      G4ThreeVector(0., 0., 10. * cm));
  // exDeuteriumIon->GetPosDist()->SetPosRot1(G4ThreeVector(0., 0., 0.));
  // exDeuteriumIon->GetPosDist()->SetPosRot2(G4ThreeVector(0., 0., 0.));
  exDeuteriumIon->GetPosDist()->ConfineSourceToVolume("NULL");

  exDeuteriumIon->GetAngDist()->SetAngDistType(
      "iso"); // Isotropic, Cosine-law, Beam, User-defined
  exDeuteriumIon->GetAngDist()->DefineAngRefAxes("angref1",
                                                 G4ThreeVector(1, 0, 0));
  exDeuteriumIon->GetAngDist()->DefineAngRefAxes("angref2",
                                                 G4ThreeVector(1, 1, 0));
  /*
    exDeuteriumIon->GetAngDist()->SetMinTheta(0);
    exDeuteriumIon->GetAngDist()->SetMaxTheta(TMath::Pi());
    exDeuteriumIon->GetAngDist()->SetMinPhi(0);
    exDeuteriumIon->GetAngDist()->SetMaxPhi(TMath::Pi() * 2.);
  */

  exDeuteriumIon->GetAngDist()->SetMinTheta(0. - 0.015);
  exDeuteriumIon->GetAngDist()->SetMaxTheta(0. + 0.015);
  exDeuteriumIon->GetAngDist()->SetMinPhi(TMath::Pi() / 2. - 0.015);
  exDeuteriumIon->GetAngDist()->SetMaxPhi(TMath::Pi() / 2. + 0.015);

  exDeuteriumIon->GetEneDist()->SetEnergyDisType(
      "Exp"); // Mono, Lin, Pow, Exp, Gaus, Brem, BBody, Cdg (cosmic diffuse
              // gamma), User, Arb, Epn (energy per nucleon)
  exDeuteriumIon->GetEneDist()->SetEzero(4.343 * MeV); // I~Exp(-E/E0)
  // ------------------------------------------------------------------------------------------------------------------------
}

MyPrimaryGenerator::~MyPrimaryGenerator() { delete fGeneralParticleSource; }

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent) {
  if (fGeneralParticleSource->GetCurrentSource()->GetParticleDefinition() ==
      G4ParticleTable::GetParticleTable()->FindParticle("e-")) {
    G4ParticleDefinition *exDIon =
        G4ParticleTable::GetParticleTable()->GetIonTable()->GetIon(1, 2,
                                                                   1.5 * MeV);
    fGeneralParticleSource->GetCurrentSource()->SetParticleDefinition(exDIon);
  }
  fGeneralParticleSource->GeneratePrimaryVertex(anEvent);

  // if (man == nullptr) man = G4AnalysisManager::Instance();
  // man->FillNtupleDColumn(0, fGeneralParticleSource->GetParticleEnergy());
  // man->AddNtupleRow(0);
}
