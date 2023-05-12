#include "construction.hh"
#include <iostream>


MyDetectorConstruction::MyDetectorConstruction()
{
        isCherenkov = true;
        isScintillator= false;

        nCols = 1;
        nRows = 1;

        fMessenger = new G4GenericMessenger(this, "/detector/", "Detector Construction");

        fMessenger->DeclareProperty("nCols", nCols, "Number of Columns");
        fMessenger->DeclareProperty("nRows", nRows, "Number of Rows");
        fMessenger->DeclareProperty("Cherenkov", isCherenkov, "Toggle Cherenkov Setup");
        fMessenger->DeclareProperty("Scintillator", isScintillator, "Toggle Scintillator Setup");

        DefineMaterials();
        xWorld = 5.*m;
        yWorld = 5.*m; //defining x and y width of the world volume in variables
        zWorld = 5.*m;

}

MyDetectorConstruction::~MyDetectorConstruction()
{}

void MyDetectorConstruction::DefineMaterials()
{
G4NistManager *nist = G4NistManager::Instance();

        SiO2 = new G4Material("SiO2", 2.201*g/cm3, 2); // name (silicon dioxide), density of material, number of components)
        SiO2->AddElement(nist->FindOrBuildElement("Si"), 1); // adds the parts required to build the above material
        SiO2->AddElement(nist->FindOrBuildElement("O"), 2);

        water = new G4Material("water", 1.000*g/cm3, 2);//water, not sure why the text is red in nano
        water->AddElement(nist->FindOrBuildElement("H"), 2);
        water->AddElement(nist->FindOrBuildElement("O"), 1);

        C = nist->FindOrBuildElement("C"); //Carbon, when making the sim it will give an error message for an unused variable, this can be ignored

        Aerogel = new G4Material("Aerogel", 0.200*g/cm3, 3);
        Aerogel->AddMaterial(SiO2, 62.5*perCent); //instead of density, if you know proportion you can do it that way
        Aerogel->AddMaterial(water, 37.4*perCent);
        Aerogel->AddElement(C, 0.1*perCent);

        worldMat = nist->FindOrBuildMaterial("G4_AIR");

        G4double energy[2] = {1.239841939*eV/0.9, 1.239841939*eV/0.2}; //conversion factor divided by wavelength in micrometer for red and blue light respe>        G4double rindexAerogel[2] = {1.1, 1.1};// refractive index of aerogel constant for this example
	G4double rindexAerogel[2] = {1.1, 1.1};
        G4double rindexWorld[2] = {1.0, 1.0};

        G4MaterialPropertiesTable *mptAerogel = new G4MaterialPropertiesTable();
        mptAerogel->AddProperty("RINDEX", energy, rindexAerogel, 2); //adding properties of the aerogel so it radiates

        G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable();
        mptWorld->AddProperty("RINDEX", energy, rindexWorld, 2); //adding properties of the air so that radiation extends past radiator

        Aerogel->SetMaterialPropertiesTable(mptAerogel);

        worldMat->SetMaterialPropertiesTable(mptWorld);
        NaI = new G4Material("NaI", 3.67*g/cm3, 2);
        NaI->AddElement(nist->FindOrBuildElement("Na"), 1);
        NaI->AddElement(nist->FindOrBuildElement("I"), 1);
}

void MyDetectorConstruction::ConstructCherenkov()
{
        solidRadiator = new G4Box("solidRadiator", 4.*m, 4.*m, 1.*m); //name, length, width, thickness (each should be smaller than the world volume, a>

        logicRadiator = new G4LogicalVolume(solidRadiator, Aerogel, "logicalRadiator");

        physRadiator = new G4PVPlacement(0, G4ThreeVector(0., 0., 2.5*m), logicRadiator, "physRadiator", logicWorld, false, 0, true);
//creating photon detectors
        solidDetector = new G4Box("solidDetector", xWorld/nRows, yWorld/nCols, 0.01*m); // (half width, half length, half thickness)

        logicDetector = new G4LogicalVolume(solidDetector, worldMat, "logicDetector");//for this example it consists of world material

        for(G4int i = 0; i < nRows; i++)
        {
                for(G4int j = 0; j < nCols; j++)
                        {
                                physDetector = new G4PVPlacement(0, G4ThreeVector(-5.*m+(i+5.)*m/nRows, -5.*m+(j+5.)*m/nCols, 4.9*m), logicDetector, "physDetector", logicWorld, false, j+i*nCols, true);//j+i*10 means when i=0, j = 0-9 then you get 10-19 when i=1 and so on to get unique numbers for each detector
                        }
        }

/*
Syntax of G4PVPlavement(rotation, placement(half width of world volume (so it starts on the side)+(shift to the right so it builds it centered instead of h>*/


}

void MyDetectorConstruction::ConstructScintillator()
{
        solidScintillator = new G4Box("solidScintillator", 5*cm, 5*cm, 6*cm);

        logicScintillator = new G4LogicalVolume(solidScintillator, NaI, "logicalScintillator");

        physScintillator = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicScintillator, "physScintillator", logicWorld, false, 0, true);
}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{

        solidWorld = new G4Box("solidWorld", xWorld, yWorld, zWorld); // width, length, height (taken in half measurements so here we get a full meter)

        logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");

        physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);

        if(isCherenkov)
                ConstructCherenkov();
        if(isScintillator)
                ConstructScintillator();
        return physWorld;
}

void MyDetectorConstruction::ConstructSDandField()
{
        MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector");

        if(isCherenkov)
                logicDetector->SetSensitiveDetector(sensDet);

}
