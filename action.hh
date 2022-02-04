#pragma once

#include "G4VUserActionInitialization.hh"

#include "generator.hh"
#include "runaction.hh"

class MyActionInitialization : public G4VUserActionInitialization {
public:
  MyActionInitialization();
  ~MyActionInitialization();

  virtual void Build() const;
};
