#include "action.hh"

MyActionInitialization::MyActionInitialization() {
}

MyActionInitialization::~MyActionInitialization() {
}

void MyActionInitialization::Build() const {
  SetUserAction(new MyPrimaryGenerator());
}
