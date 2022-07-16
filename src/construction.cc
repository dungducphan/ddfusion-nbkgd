#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction() : mBeFlag(true) {
    mMessenger = new G4GenericMessenger(this, "/detCon/", "Detector Construction");
    mMessenger->DeclareProperty("BeFlag", mBeFlag, "Flag of having Be converter block");
}

MyDetectorConstruction::~MyDetectorConstruction() {
}

G4LogicalVolume* MyDetectorConstruction::GetLogicalRCF(G4int thickness, G4Material* material) {
    G4Box*  box = new G4Box(Form("box%i", thickness),23.1*mm,27.4*mm,0.5 * (G4double) thickness *
    um);
    G4Tubs* cyl = new G4Tubs(Form("cyl%i", thickness),0,2.8*mm,0.5 * (G4double) thickness * um,0,
                             TMath::TwoPi());
    G4SubtractionSolid* solidRCF = new G4SubtractionSolid(Form("solidRCF%i", thickness), box, cyl);
    return new G4LogicalVolume(solidRCF, material, Form("logicRCF_%i_%s", thickness, (material->GetName()).c_str()));
}

G4VPhysicalVolume *MyDetectorConstruction::Construct() {
    //---- Define materials ----
    G4NistManager *nist = G4NistManager::Instance();
    vacuum = nist->FindOrBuildMaterial("G4_Galactic");
    worldMat = nist->FindOrBuildMaterial("G4_AIR");
    beConverterMat = nist->FindOrBuildMaterial("G4_Be");
    aluminumMat = nist->FindOrBuildMaterial("G4_Al");
    copperMat = nist->FindOrBuildMaterial("G4_Cu");
    mylar = nist->FindOrBuildMaterial("G4_MYLAR");


    //---- Define volumes ----
    // // World
    G4Box *solidWorld = new G4Box("solidWorld", 5 * m, 1.5 * m, 5 * m);
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicWorld, "physWorld", 0, false, 0,
                                                     true);

    // // Vacuum World
    G4Box *solidVacuumWorld = new G4Box("solidVacuumWorld", 1.2 * m, 1.2 * m, 1.2 * m);
    G4LogicalVolume *logicVacuumWorld = new G4LogicalVolume(solidVacuumWorld, vacuum, "logicVacuumWorld");
    G4VPhysicalVolume *physVacuumWorld = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicVacuumWorld, "physVacuumWorld", logicWorld, false, 0,
                                                     true);

    /*
    // // Be-converter
    G4Box *solidBeConverter = new G4Box("solidBeConverter", 0.5 * cm, 0.5 * cm, 2.5 * cm);
    logicBeConverter = new G4LogicalVolume(solidBeConverter, beConverterMat, "logicBeConverter");
    physBeConverter = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicBeConverter, "physBeConverter", logicVacuumWorld, false, 0, true);
    */

    // // ------------------------------------------------------------------------------------------
    // // BEGIN RCF
    // 13 µm Al
    // 109 µm Mylar
    // 8x (100µm Al+ 109µm Mylar)
    // 6x (150 µm Cu + 260 µm Mylar)
    // 16x (500 µm Cu + 280µm Mylar)
    // 5x (1mm Cu + 280µm Mylar)

    logicRCF_13_Al = GetLogicalRCF(13, aluminumMat);
    logicRCF_109_Mylar = GetLogicalRCF(109, mylar);
    logicRCF_100_Al = GetLogicalRCF(100, aluminumMat);
    logicRCF_150_Cu = GetLogicalRCF(150, copperMat);
    logicRCF_500_Cu = GetLogicalRCF(500, copperMat);
    logicRCF_1000_Cu = GetLogicalRCF(1000, copperMat);
    logicRCF_280_Mylar = GetLogicalRCF(280, mylar);
    logicRCF_260_Mylar = GetLogicalRCF(260, mylar);

    int icopy_RCF = 1000;
    G4double z_RCF_offset = 0.7 * cm;
    G4double z_slab01 = z_RCF_offset - (13. / 2.) * um;
    new G4PVPlacement(0, G4ThreeVector(0., 0., z_slab01), logicRCF_13_Al,     "physSLAB01",
                      logicVacuumWorld, false, icopy_RCF++, true);

    G4double z_slab02 = z_slab01 - (13. / 2.) * um - (109. / 2.) * um;
    new G4PVPlacement(0, G4ThreeVector(0., 0., z_slab02), logicRCF_109_Mylar, "physSLAB02",
                      logicVacuumWorld, false, icopy_RCF++, true);

    G4double z_slab03 = z_slab02;
    for (unsigned int i = 0; i < 8; i++) {
        z_slab03 = z_slab03 - (109. / 2.) * um - (100. / 2.) * um;
        new G4PVPlacement(0, G4ThreeVector(0., 0., z_slab03), logicRCF_100_Al, Form
        ("physSLAB03_%i", i+1),logicVacuumWorld, false, icopy_RCF++, true);
        z_slab03 = z_slab03 - (100. / 2.) * um - (109. / 2.) * um;
        new G4PVPlacement(0, G4ThreeVector(0., 0., z_slab03), logicRCF_109_Mylar, Form
                ("physSLAB03_%i", i+1),logicVacuumWorld, false, icopy_RCF++, true);
    }

    G4double z_slab04 = z_slab03;
    for (unsigned int i = 0; i < 6; i++) {
        if (i == 0) {
            z_slab04 = z_slab04 - (109. / 2.) * um - (150. / 2.) * um;
        } else {
            z_slab04 = z_slab04 - (260. / 2.) * um - (150. / 2.) * um;
        }
        new G4PVPlacement(0, G4ThreeVector(0., 0., z_slab04), logicRCF_150_Cu, Form
                ("physSLAB04_%i", i+1),logicVacuumWorld, false, icopy_RCF++, true);
        z_slab04 = z_slab04 - (150. / 2.) * um - (260. / 2.) * um;
        new G4PVPlacement(0, G4ThreeVector(0., 0., z_slab04), logicRCF_260_Mylar, Form
                ("physSLAB04_%i", i+1),logicVacuumWorld, false, icopy_RCF++, true);
    }

    G4double z_slab05 = z_slab04;
    for (unsigned int i = 0; i < 16; i++) {
        if (i == 0) {
            z_slab05 = z_slab05 - (260. / 2.) * um - (500. / 2.) * um;
        } else {
            z_slab05 = z_slab05 - (280. / 2.) * um - (500. / 2.) * um;
        }
        new G4PVPlacement(0, G4ThreeVector(0., 0., z_slab05), logicRCF_500_Cu, Form
                ("physSLAB05_%i", i+1),logicVacuumWorld, false, icopy_RCF++, true);
        z_slab05 = z_slab05 - (500. / 2.) * um - (280. / 2.) * um;
        new G4PVPlacement(0, G4ThreeVector(0., 0., z_slab05), logicRCF_280_Mylar, Form
                ("physSLAB05_%i", i+1),logicVacuumWorld, false, icopy_RCF++, true);
    }

    G4double z_slab06 = z_slab05;
    for (unsigned int i = 0; i < 5; i++) {
        z_slab06 = z_slab06 - (280. / 2.) * um - (1000. / 2.) * um;
        new G4PVPlacement(0, G4ThreeVector(0., 0., z_slab06), logicRCF_1000_Cu, Form
                ("physSLAB06_%i", i+1),logicVacuumWorld, false, icopy_RCF++, true);
        z_slab06 = z_slab06 - (1000. / 2.) * um - (280. / 2.) * um;
        new G4PVPlacement(0, G4ThreeVector(0., 0., z_slab06), logicRCF_280_Mylar, Form
                ("physSLAB06_%i", i+1),logicVacuumWorld, false, icopy_RCF++, true);
    }
    // // END RCF
    // // ------------------------------------------------------------------------------------------

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
    new G4PVPlacement(tc1_transform3D, logicTC1, "physTC1", logicVacuumWorld, false, 0, true);

    // Optical table
    G4Tubs *solidOpTable = new G4Tubs("solidOpTable", 0, 0.95 * m, 0.5 * cm, 0, 2 * TMath::Pi());
    G4LogicalVolume *logicOpTable = new G4LogicalVolume(solidOpTable, aluminumMat, "logicOpTable");
    G4RotationMatrix *rotMatOpTable = new G4RotationMatrix(0, TMath::Pi() / 2, 0);
    new G4PVPlacement(rotMatOpTable, G4ThreeVector(0., -30. * cm, 0.), logicOpTable, "physOpTable", logicVacuumWorld, false, 0,true);

    // // Sensitive Detector
    // Dimension info from BTI: https://bubbletech.ca/product/bdt/
    G4Tubs *solidSD = new G4Tubs("solidSD", 0, 4.5 * mm, 72.5 * mm, 0, TMath::TwoPi());
    G4LogicalVolume *logicSD = new G4LogicalVolume(solidSD, worldMat, "logicSD");

    std::vector<double> angles = {0., 64., 154., 184., 224., 282., 324.};
    for (unsigned int i = 0; i < angles.size(); i++) {
        angles.at(i) = angles.at(i) + 180;
        double zi = (1 + 0.05) * m * TMath::Cos(angles.at(i) * degree);
        double xi = (1 + 0.05) * m * TMath::Sin(angles.at(i) * degree);
        G4RotationMatrix *rotMat = new G4RotationMatrix(90 * degree, (90 + angles.at(i)) * degree, 0.);
        new G4PVPlacement(rotMat, G4ThreeVector(xi, 0, zi), logicSD, Form("physSD_%03i", i+1), logicVacuumWorld, false, i+1, true);
    }
    for (unsigned int i = 0; i < angles.size(); i++) {
        double zi = (1 + 0.05) * m * TMath::Cos(angles.at(i) * degree);
        double xi = (1 + 0.05) * m * TMath::Sin(angles.at(i) * degree);
        G4RotationMatrix *rotMat = new G4RotationMatrix(90 * degree, (90 + angles.at(i)) * degree, 0.);
        new G4PVPlacement(rotMat, G4ThreeVector(xi, 1.05 * m, zi), logicSD, Form("physSD_%03i", i+8), logicVacuumWorld, false, i+8, true);
    }

    // TOF
    G4Tubs *solidTOFSD = new G4Tubs("solidTOFSD", 0,
                                    2.54*cm,
                                    1 * cm,
                                    0,
                                    2*TMath::Pi());
    G4LogicalVolume *logicTOFSD = new G4LogicalVolume(solidTOFSD, worldMat, "logicTOFSD");
    double ztof = (4.4) * m * TMath::Cos((180 - 110) * degree);
    double xtof = (4.4) * m * TMath::Sin((180 - 110) * degree);
    G4RotationMatrix *rotTOFMat = new G4RotationMatrix(TMath::PiOver2(), (180 - 110) * degree, 0.);
    new G4PVPlacement(rotTOFMat, G4ThreeVector(xtof, 0, ztof), logicTOFSD, Form("physSD_%03i", 15), logicWorld, false, 15, true);

    return physWorld;
}

void MyDetectorConstruction::ConstructSDandField() {
    const G4String name_nSD = "NeutronSD";
    NeutronSD *nsd = new NeutronSD(name_nSD);
    G4SDManager::GetSDMpointer()->AddNewDetector(nsd);
    SetSensitiveDetector("logicSD", nsd);
    SetSensitiveDetector("logicTOFSD", nsd);

    const G4String name_DSD = "DeuteronSD";
    DeuteronSD *dsd = new DeuteronSD(name_DSD);
    G4SDManager::GetSDMpointer()->AddNewDetector(dsd);
    SetSensitiveDetector(logicRCF_109_Mylar, dsd);
    SetSensitiveDetector(logicRCF_280_Mylar, dsd);
    SetSensitiveDetector(logicRCF_260_Mylar, dsd);
}
