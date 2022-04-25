#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator() {
  fGeneralParticleSource = new G4GeneralParticleSource();

  // First source: excited deuterium nuclei
  // ------------------------------------------------------------------------------------------------------------------------
  G4SingleParticleSource *exDeuteriumIon =
      fGeneralParticleSource->GetCurrentSource();

  // Initiate the particles as electrons
  // Re-define them as D-ion later (there are some issue with GenericIon is not ready at this stage of initialization).
  G4ParticleDefinition *exDIon =
      G4ParticleTable::GetParticleTable()->FindParticle("e-");

  exDeuteriumIon->SetParticleDefinition(exDIon);
  exDeuteriumIon->SetNumberOfParticles(10);

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


  exDeuteriumIon->GetEneDist()->SetEnergyDisType("User"); // Mono, Lin, Pow, Exp, Gaus, Brem, BBody, Cdg (cosmic diffuse gamma), User, Arb, Epn (energy per nucleon)
  //  exDeuteriumIon->GetEneDist()->SetEzero(4.343 * MeV); // I~Exp(-E/E0)
  SetEnergySpectrum(exDeuteriumIon);
  // ------------------------------------------------------------------------------------------------------------------------
}

void MyPrimaryGenerator::SetEnergySpectrum(G4SingleParticleSource* sps) {
    std::ifstream inSpec("./DSpec.csv");
    double e_tmp;
    double flux_tmp;
    while (inSpec >> e_tmp >> flux_tmp) {
        G4ThreeVector binData(e_tmp*MeV, flux_tmp * 1023. / 4.99304e+13, 0);
        sps->GetEneDist()->UserEnergyHisto(binData);
    }
    inSpec.close();
}

MyPrimaryGenerator::~MyPrimaryGenerator() { delete fGeneralParticleSource; }

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent) {
  // Re-define the particles as D-ion here
  if (fGeneralParticleSource->GetCurrentSource()->GetParticleDefinition() ==
      G4ParticleTable::GetParticleTable()->FindParticle("e-")) {
    G4ParticleDefinition *exDIon =
        G4ParticleTable::GetParticleTable()->GetIonTable()->GetIon(1, 2,
                                                                   1.5 * MeV);
    fGeneralParticleSource->GetCurrentSource()->SetParticleDefinition(exDIon);
  }

  // Printing progress every 20000 events
  G4int evtNo = anEvent->GetEventID();
  if (evtNo % 20000 == 0) G4cout << Form("Event number %08i.", evtNo) << G4endl;

  // Generate primary vertex here
  fGeneralParticleSource->GeneratePrimaryVertex(anEvent);

  // if (man == nullptr) man = G4AnalysisManager::Instance();
  // man->FillNtupleDColumn(0, fGeneralParticleSource->GetParticleEnergy());
  // man->AddNtupleRow(0);
}
