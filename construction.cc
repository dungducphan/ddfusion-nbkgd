#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction() {
}

MyDetectorConstruction::~MyDetectorConstruction() {
}

G4VPhysicalVolume* MyDetectorConstruction::Construct() {
  //---- Define materials ---- 
  G4NistManager * nist = G4NistManager::Instance();
  
  // // World material 
  G4Material * worldMat = nist->FindOrBuildMaterial("G4_AIR");

  // // Be-converter material
  G4Material * beConverterMat = nist->FindOrBuildMaterial("G4_Be");

  //---- Define volumes ----
  // // World
  G4Box *solidWorld = new G4Box("solidWorld", 0.5*m, 0.5*m, 0.5*m);
  G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
  G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicWorld, "physWorld", 0, false, 0, true);
 
  // // Be-converter
  G4Box *solidBeConverter = new G4Box("solidBeConverter", 0.5*cm, 0.5*cm, 2.5*cm);
  G4LogicalVolume *logicBeConverter = new G4LogicalVolume(solidBeConverter, beConverterMat, "logicBeConverter");
  G4VPhysicalVolume *physBeConverter = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicBeConverter, "physBeConverter", logicWorld, false, 0, true);

  return physWorld;
}
