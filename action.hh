#pragma once

#include "G4VUserActionInitialization.hh"

#include "generator.hh"
#include "runaction.hh"

class MyRunAction;

class MyActionInitialization : public G4VUserActionInitialization {
public:
  MyActionInitialization(std::vector<std::pair<G4double, G4double>> gunEne);
  ~MyActionInitialization();

  virtual void Build() const;
  virtual void BuildForMaster() const;

  std::vector<std::pair<G4double, G4double>> gunEneDist;
};
