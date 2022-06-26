#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator(std::vector<std::pair<G4double, G4double>> gunEne) {
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
    exDeuteriumIon->SetNumberOfParticles(10000);

    exDeuteriumIon->GetPosDist()->SetPosDisType(
            "Volume"); // Point, Beam, Plane, Surface, Volume
    exDeuteriumIon->GetPosDist()->SetPosDisShape(
            "Sphere"); // Square, Circle, Ellipse, Rectangle, Sphere, Ellipsoid,
    // Cylinder, Parallelepiped
    exDeuteriumIon->GetPosDist()->SetRadius(3 * um);
    exDeuteriumIon->GetPosDist()->SetCentreCoords(
            G4ThreeVector(0., 0., 5.2 * cm));
    // exDeuteriumIon->GetPosDist()->SetPosRot1(G4ThreeVector(0., 0., 0.));
    // exDeuteriumIon->GetPosDist()->SetPosRot2(G4ThreeVector(0., 0., 0.));
    exDeuteriumIon->GetPosDist()->ConfineSourceToVolume("NULL");

    exDeuteriumIon->GetAngDist()->SetAngDistType(
            "iso"); // Isotropic, Cosine-law, Beam, User-defined
    exDeuteriumIon->GetAngDist()->DefineAngRefAxes("angref1",
                                                   G4ThreeVector(0, 0, 1));
    exDeuteriumIon->GetAngDist()->DefineAngRefAxes("angref2",
                                                   G4ThreeVector(1, 0, 0));
/*
    // Isotropic deuteron source
    exDeuteriumIon->GetAngDist()->SetMinTheta(0);
    exDeuteriumIon->GetAngDist()->SetMaxTheta(TMath::Pi());
    exDeuteriumIon->GetAngDist()->SetMinPhi(0);
    exDeuteriumIon->GetAngDist()->SetMaxPhi(TMath::Pi() * 2.);
*/

/*
    // Deuteron beam source
    exDeuteriumIon->GetAngDist()->SetMinTheta(0);
    exDeuteriumIon->GetAngDist()->SetMaxTheta(TMath::PiOver2() / 6);
    exDeuteriumIon->GetAngDist()->SetMinPhi(0);
    exDeuteriumIon->GetAngDist()->SetMaxPhi(TMath::TwoPi());
*/

    // Deuteron ring source
    exDeuteriumIon->GetAngDist()->SetMinTheta(TMath::PiOver2() - TMath::PiOver2()/12);
    exDeuteriumIon->GetAngDist()->SetMaxTheta(TMath::PiOver2() + TMath::PiOver2()/12);
    exDeuteriumIon->GetAngDist()->SetMinPhi(0);
    exDeuteriumIon->GetAngDist()->SetMaxPhi(TMath::TwoPi());

/*
 * Gaussian energy distribution
    //  exDeuteriumIon->GetEneDist()->SetEnergyDisType("Gauss"); // Mono, Lin, Pow, Exp, Gaus, Brem, BBody, Cdg (cosmic diffuse gamma), User, Arb, Epn (energy per nucleon)
    //  exDeuteriumIon->GetEneDist()->SetMonoEnergy(2.2 * MeV);
    //  exDeuteriumIon->GetEneDist()->SetBeamSigmaInE(0.1 * MeV);
*/

    // Realistic energy distribution from Thompson-Parabolla Spectrometer
    exDeuteriumIon->GetEneDist()->SetEnergyDisType("User");
    for (int i = 0; i < gunEne.size(); i++) {
        G4double ehi = std::get<0>(gunEne.at(i));
        G4double val = std::get<1>(gunEne.at(i));
        exDeuteriumIon->GetEneDist()->UserEnergyHisto(G4ThreeVector(ehi, val, 0));
    }
    // ------------------------------------------------------------------------------------------------------------------------
}

MyPrimaryGenerator::~MyPrimaryGenerator() { delete fGeneralParticleSource; }

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent) {
    // Re-define the particles as D-ion here
    if (fGeneralParticleSource->GetCurrentSource()->GetParticleDefinition() ==
        G4ParticleTable::GetParticleTable()->FindParticle("e-")) {
        G4ParticleDefinition *exDIon =
                G4ParticleTable::GetParticleTable()->GetIonTable()->GetIon(1, 2,
                                                                           3 * MeV);
        fGeneralParticleSource->GetCurrentSource()->SetParticleDefinition(exDIon);
    }

    // Printing progress every 20000 events
    G4int evtNo = anEvent->GetEventID();
    if (evtNo % 1000 == 0) G4cout << Form("Event number %08i.", evtNo) << G4endl;

    // Generate primary vertex here
    fGeneralParticleSource->GeneratePrimaryVertex(anEvent);
}
