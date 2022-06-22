#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction() : mBeFlag(true) {
    mMessenger = new G4GenericMessenger(this, "/detCon/", "Detector Construction");
    mMessenger->DeclareProperty("BeFlag", mBeFlag, "Flag of having Be converter block");
}

MyDetectorConstruction::~MyDetectorConstruction() {
}

G4VPhysicalVolume *MyDetectorConstruction::Construct() {
    //---- Define materials ----
    G4NistManager *nist = G4NistManager::Instance();

    // // World material
    G4Material *worldMat = nist->FindOrBuildMaterial("G4_Galactic");

    // // Be-converter material
    G4Material *beConverterMat = nist->FindOrBuildMaterial("G4_Be");

    // // Aluminum
    G4Material *aluminumMat = nist->FindOrBuildMaterial("G4_Al");

    //---- Define volumes ----
    // // World
    G4Box *solidWorld = new G4Box("solidWorld", 1.5 * m, 1.5 * m, 1.5 * m);
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicWorld, "physWorld", 0, false, 0,
                                                     true);

    // // Be-converter
    G4Box *solidBeConverter = new G4Box("solidBeConverter", 0.5 * cm, 0.5 * cm, 2.5 * cm);
    G4LogicalVolume *logicBeConverter = new G4LogicalVolume(solidBeConverter, beConverterMat, "logicBeConverter");
    if (mBeFlag) {
        new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicBeConverter, "physBeConverter", logicWorld, false, 0,
                          true);
    }

    // // TC1 container
    G4Tubs *solidOuterTubs = new G4Tubs("solidOuterTubs", 0, 1 * m, 1 * m, 0, 2 * TMath::Pi());
    G4Tubs *solidInnerTubs = new G4Tubs("solidInnerTubs", 0, (1 - 0.03) * m, (1 - 0.03) * m, 0, 2 * TMath::Pi());
    G4SubtractionSolid *solidTC1 = new G4SubtractionSolid("solidTC1", solidOuterTubs, solidInnerTubs);
    G4LogicalVolume *logicTC1 = new G4LogicalVolume(solidTC1, aluminumMat, "logicTC1");
    G4double phi = TMath::Pi() / 2.;
    G4double cp = TMath::Cos(phi);
    G4double sp = TMath::Sin(phi);
    G4ThreeVector u = G4ThreeVector(1, 0, 0);
    G4ThreeVector v = G4ThreeVector(0, cp, -sp);
    G4ThreeVector w = G4ThreeVector(0, sp, cp);
    const G4Transform3D tc1_transform3D = G4Transform3D(G4RotationMatrix(u, v, w), G4ThreeVector(0., 0., 0.));
    new G4PVPlacement(tc1_transform3D, logicTC1, "physTC1", logicWorld, false, 0, true);

    // Optical table
    G4Tubs *solidOpTable = new G4Tubs("solidOpTable", 0, 0.95 * m, 0.5 * cm, 0, 2 * TMath::Pi());
    G4LogicalVolume *logicOpTable = new G4LogicalVolume(solidOpTable, aluminumMat, "logicOpTable");
    G4RotationMatrix *rotMatOpTable = new G4RotationMatrix(0, TMath::Pi() / 2, 0);
    new G4PVPlacement(rotMatOpTable, G4ThreeVector(0., -10. * cm, 0.), logicOpTable, "physOpTable", logicWorld, false, 0,true);

    // // Sensitive Detector
    // Dimension info from BTI: https://bubbletech.ca/product/bdt/
    G4Tubs *solidSD = new G4Tubs("solidSD", 0, 4.5 * mm, 72.5 * mm, 0, TMath::TwoPi());
    G4LogicalVolume *logicSD = new G4LogicalVolume(solidSD, worldMat, "logicSD");
    double angle = TMath::Pi()/6;
    G4RotationMatrix *rotMat = new G4RotationMatrix(0, TMath::Pi() / 2, 0);
    for (unsigned int i = 0; i < 6; i++) {
        angle += TMath::Pi() / 3;
        // FIXME
        double xi = (1 + 0.05) * m * TMath::Cos(angle);
        double zi = (1 + 0.05) * m * TMath::Sin(angle);
        new G4PVPlacement(rotMat, G4ThreeVector(xi, 0., zi), logicSD, Form("physSD_%03i", i), logicWorld, false, i,
                          true);
    }

    return physWorld;
}

void MyDetectorConstruction::ConstructSDandField() {
    const G4String name = "NeutronSD";
    NeutronSD *sd = new NeutronSD(name);
    G4SDManager::GetSDMpointer()->AddNewDetector(sd);
    SetSensitiveDetector("logicSD", sd);
}
