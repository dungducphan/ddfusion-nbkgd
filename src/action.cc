#include "action.hh"

MyActionInitialization::MyActionInitialization(std::vector<std::pair<G4double, G4double>> gunEne) {
    gunEneDist = gunEne;
}

MyActionInitialization::~MyActionInitialization() {
}

void MyActionInitialization::Build() const {
  SetUserAction(new MyRunAction());
  SetUserAction(new MyPrimaryGenerator(gunEneDist));
}

void MyActionInitialization::BuildForMaster() const {
  SetUserAction(new MyRunAction());
}
