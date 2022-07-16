#pragma once

#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4SubtractionSolid.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4SystemOfUnits.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4GenericMessenger.hh"

#include "TMath.h"
#include "TString.h"

#include "neutronsd.hh"
#include "deuteronsd.hh"

class MyDetectorConstruction : public G4VUserDetectorConstruction {
public:
    MyDetectorConstruction();
    ~MyDetectorConstruction();

    virtual G4VPhysicalVolume *Construct();
    virtual void ConstructSDandField();

    G4LogicalVolume* GetLogicalRCF(G4int thickness, G4Material* material);

    G4GenericMessenger *mMessenger;
    G4bool mBeFlag;

    G4Material *worldMat;
    G4Material *beConverterMat;
    G4Material *aluminumMat;
    G4Material *mylar;
    G4Material *copperMat;
    G4Material *vacuum;

    G4LogicalVolume *logicWorld;
    G4VPhysicalVolume *physWorld;

    G4LogicalVolume *logicRCF;
    G4VPhysicalVolume *physRCF;

    G4LogicalVolume *logicBeConverter;
    G4VPhysicalVolume *physBeConverter;

    G4LogicalVolume* logicRCF_13_Al;
    G4LogicalVolume* logicRCF_109_Mylar;
    G4LogicalVolume* logicRCF_100_Al;
    G4LogicalVolume* logicRCF_150_Cu;
    G4LogicalVolume* logicRCF_500_Cu;
    G4LogicalVolume* logicRCF_1000_Cu;
    G4LogicalVolume* logicRCF_280_Mylar;
    G4LogicalVolume* logicRCF_260_Mylar;
};
