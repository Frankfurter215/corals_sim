#include "generator.hh"
MyPrimaryGenerator::MyPrimaryGenerator()
{
        fParticleGun = new G4ParticleGun(1); //1 particle per event

        G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
        G4ParticleDefinition *particle = particleTable->FindParticle("geantino");

        G4ThreeVector pos(0., -4.*m, 0.); //created in center of mother volume
        G4ThreeVector mom(0., 3., 1.73); //sets momentum direction in positive set direction

        fParticleGun->SetParticlePosition(pos);
        fParticleGun->SetParticleMomentumDirection(mom); //again, just sets the direction of momentum doesn't create momentum
        fParticleGun->SetParticleMomentum(10.*GeV); //set particle at very high momentum (original units meV)
//average particle for Corals will be 10000000000.*GeV, but this produces an ungodly amount of photons- the sim will run for more than 10 mins per event, so set it lower for testing
        fParticleGun->SetParticleDefinition(particle);

}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
        delete fParticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
        G4ParticleDefinition *particle = fParticleGun->GetParticleDefinition();

        if(particle == G4Geantino::Geantino())
        {
                G4int Z = 1;
                G4int A = 1;

                G4double charge = 1. *eplus;
                G4double energy = 1000. *keV;

                G4ParticleDefinition *ion = G4IonTable::GetIonTable() -> GetIon(Z, A, energy);
                fParticleGun->SetParticleDefinition(ion);
                fParticleGun->SetParticleCharge(charge);
        }

        fParticleGun->GeneratePrimaryVertex(anEvent);
}
