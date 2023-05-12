#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh" // includes more  units for meters since G4 runs in mm - change units with <#>*<unit>
#include "G4GenericMessenger.hh"
#include "detector.hh"
/*
Author: Frank
*/

class MyDetectorConstruction : public G4VUserDetectorConstruction
{
public:
        MyDetectorConstruction();
        ~MyDetectorConstruction();
        virtual G4VPhysicalVolume *Construct();

private:
        G4LogicalVolume *logicDetector;
        virtual void ConstructSDandField();

        G4int nCols, nRows;

        G4Box *solidWorld, *solidRadiator, *solidDetector, *solidScintillator;
        G4LogicalVolume *logicWorld, *logicRadiator, *logicScintillator;
        G4VPhysicalVolume *physWorld, *physRadiator, *physDetector, *physScintillator;

        G4GenericMessenger *fMessenger;

        G4Material *SiO2, *water, *Aerogel, *worldMat, *NaI;
        G4Element *C, *Na, *I;

        void DefineMaterials();

        void ConstructCherenkov();
        void ConstructScintillator();

        G4double xWorld, yWorld, zWorld;
        G4bool isCherenkov, isScintillator;
};

#endif
