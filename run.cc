#include "run.hh"

MyRunAction::MyRunAction()
{
        G4AnalysisManager *man = G4AnalysisManager::Instance();

        man->CreateNtuple("Photons", "Photons");
        man->CreateNtupleIColumn("fEvent"); //stands for Integer column
        man->CreateNtupleDColumn("fX");
        man->CreateNtupleDColumn("fY");
        man->CreateNtupleDColumn("fZ"); //these show position
        man->CreateNtupleDColumn("fwlen"); //shows wavelength
        man->FinishNtuple(0); //this first set (ntuple) is labelled as number 0

        man->CreateNtuple("Hits", "Hits");
        man->CreateNtupleIColumn("fEvent"); //stands for Integer column
        man->CreateNtupleDColumn("fX");
        man->CreateNtupleDColumn("fY");
        man->CreateNtupleDColumn("fZ"); //these show position
        man->FinishNtuple(1); //this second set (ntuple) is labelled as number 1

        man->CreateNtuple("Scoring", "Scoring");
        man->CreateNtupleDColumn("fEdep");
        man->FinishNtuple(2); //ntuple for nrg deposition
}

MyRunAction::~MyRunAction()
{}

void MyRunAction::BeginOfRunAction(const G4Run* run)
{
        G4AnalysisManager *man = G4AnalysisManager::Instance();

        G4int runID = run->GetRunID();

        std::stringstream strRunID;
        strRunID << runID;
//        std::cout << runID;
//      man->OpenFile("test"+strRunID.str()+".root");
        man->OpenFile("10GeV.root");
}

void MyRunAction::EndOfRunAction(const G4Run*)
{
        G4AnalysisManager *man = G4AnalysisManager::Instance();

        man->Write(); //root files are sensitive, if you don't do this it will corrupt the data
        man->CloseFile();
}
