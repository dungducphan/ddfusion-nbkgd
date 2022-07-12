#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction() : mBeFlag(true) {
    mMessenger = new G4GenericMessenger(this, "/detCon/", "Detector Construction");
    mMessenger->DeclareProperty("BeFlag", mBeFlag, "Flag of having Be converter block");
}

MyDetectorConstruction::~MyDetectorConstruction() {
}

G4LogicalVolume* MyDetectorConstruction::GetLogicalRCF(G4int thickness, G4Material* material) {
    G4Box*  box = new G4Box(Form("box%i", thickness),23.1*mm,27.4*mm,(G4double) thickness *um);
    G4Tubs* cyl = new G4Tubs(Form("cyl%i", thickness),0,2.8*mm,(G4double) thickness *um,0,TMath::TwoPi());
    G4SubtractionSolid* solidRCF = new G4SubtractionSolid(Form("solidRCF%i", thickness), box, cyl);
    return new G4LogicalVolume(solidRCF, material, Form("logicRCF_%i_%s", thickness, (material->GetName()).c_str()));
}

G4VPhysicalVolume *MyDetectorConstruction::Construct() {
    //---- Define materials ----
    G4NistManager *nist = G4NistManager::Instance();
    worldMat = nist->FindOrBuildMaterial("G4_Galactic");
    beConverterMat = nist->FindOrBuildMaterial("G4_Be");
    aluminumMat = nist->FindOrBuildMaterial("G4_Al");
    copperMat = nist->FindOrBuildMaterial("G4_Cu");
    mylar = nist->FindOrBuildMaterial("G4_MYLAR");

    //---- Define volumes ----
    // // World
    G4Box *solidWorld = new G4Box("solidWorld", 1.5 * m, 1.5 * m, 1.5 * m);
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicWorld, "physWorld", 0, false, 0,
                                                     true);

    // // Be-converter
    /*
    G4Box *solidBeConverter = new G4Box("solidBeConverter", 0.5 * cm, 0.5 * cm, 2.5 * cm);
    logicBeConverter = new G4LogicalVolume(solidBeConverter, beConverterMat, "logicBeConverter");
    physBeConverter = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicBeConverter, "physBeConverter", logicWorld, false, 0, true);
    */

    // // RCF
    logicRCF_13_Al = GetLogicalRCF(13, aluminumMat);
    logicRCF_109_Mylar = GetLogicalRCF(109, mylar);
    logicRCF_100_Al = GetLogicalRCF(100, aluminumMat);
    logicRCF_150_Cu = GetLogicalRCF(150, copperMat);
    logicRCF_1000_Cu = GetLogicalRCF(1000, copperMat);
    logicRCF_280_Mylar = GetLogicalRCF(280, mylar);

    G4double z_RCF_offset = (2.3 / 2.) * cm;
    G4double z_slab01 = z_RCF_offset;
    G4double z_slab02 = z_slab01 - (13. / 2.) * um - (109. / 2.) * um;
    G4double z_slab03 = z_slab02 - (109. / 2.) * um - (100. / 2.) * um;

    new G4PVPlacement(0, G4ThreeVector(0., 0., z_slab01), logicRCF_13_Al,     "physSLAB01", logicWorld, false, 0, true);
    new G4PVPlacement(0, G4ThreeVector(0., 0., z_slab02), logicRCF_109_Mylar, "physSLAB02", logicWorld, false, 0, true);
    new G4PVPlacement(0, G4ThreeVector(0., 0., z_slab03), logicRCF_100_Al,    "physSLAB03", logicWorld, false, 0, true);











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
    new G4PVPlacement(rotMatOpTable, G4ThreeVector(0., -30. * cm, 0.), logicOpTable, "physOpTable", logicWorld, false, 0,true);

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
    new G4PVPlacement(0, G4ThreeVector(0, 1.05 * m, 0.), logicSD, Form("physSD_%03i", 6), logicWorld, false, 6, true);

    return physWorld;
}

void MyDetectorConstruction::ConstructSDandField() {
    const G4String name = "NeutronSD";
    NeutronSD *sd = new NeutronSD(name);
    G4SDManager::GetSDMpointer()->AddNewDetector(sd);
    SetSensitiveDetector("logicSD", sd);
}
