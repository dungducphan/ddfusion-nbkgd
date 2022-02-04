#include "action.hh"

MyActionInitialization::MyActionInitialization() {
}

MyActionInitialization::~MyActionInitialization() {
}

void MyActionInitialization::Build() const {
  SetUserAction(new MyRunAction());
  SetUserAction(new MyPrimaryGenerator());
}
