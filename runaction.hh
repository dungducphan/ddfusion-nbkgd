#pragma once 

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "G4AnalysisManager.hh"

class G4Run;

class MyRunAction : public G4UserRunAction {
  public:
    MyRunAction();
    virtual ~MyRunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);

  private:
};
