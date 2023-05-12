#include "physics.hh"

MyPhysicsList::MyPhysicsList()
{
        RegisterPhysics (new G4EmStandardPhysics()); //only use physics that is required b/c the more you implement the longer it takes to compile
        RegisterPhysics (new G4OpticalPhysics());
        RegisterPhysics (new G4DecayPhysics());
        RegisterPhysics (new G4RadioactiveDecayPhysics());
}

MyPhysicsList::~MyPhysicsList()
{}
