#ifndef ACTION_HH
#define ACTION_HH

#include "G4VUserActionInitialization.hh"

#include "generator.hh"

#include"run.hh"

class MyActionInitialization : public G4VUserActionInitialization
{
public:
        MyActionInitialization();
        ~MyActionInitialization();

        virtual void Build() const; //runs partical gun for each thread
        virtual void BuildForMaster() const; //runs partical gun for all threads
};

#endif
