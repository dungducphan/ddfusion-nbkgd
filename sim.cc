#include <iostream>

#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"

#include "construction.hh"
#include "physics.hh"
#include "action.hh"

int main(int argc, char **argv) {

    G4UIExecutive *ui = nullptr;
    if (argc == 1) {
        ui = new G4UIExecutive(argc, argv);
    }

#ifdef G4MULTITHREADED
    G4MTRunManager *runManager = new G4MTRunManager();
#else
    G4RunManager *runManager = new G4RunManager();
#endif

    runManager->SetUserInitialization(new MyDetectorConstruction());
    runManager->SetUserInitialization(new MyPhysicsList());

    std::vector<std::pair<G4double, G4double>> gunEne;
    std::ifstream inSpec("./DSpec.csv");
    double e_tmp;
    double flux_tmp;
    while (inSpec >> e_tmp >> flux_tmp) {
        std::cout << e_tmp << flux_tmp << std::endl;
        gunEne.push_back(std::make_pair(e_tmp, flux_tmp));
    }
    inSpec.close();
    runManager->SetUserInitialization(new MyActionInitialization(gunEne));

    G4VisManager *visManager = new G4VisExecutive();
    visManager->Initialize();

    G4UImanager *uiManager = G4UImanager::GetUIpointer();

    if (!ui) {
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        uiManager->ApplyCommand(command + fileName);
    } else {
        runManager->Initialize();
        G4String command = "/control/execute ";
        uiManager->ApplyCommand("/control/execute vis.mac");
        ui->SessionStart();
        delete ui;
    }

    delete visManager;
    delete runManager;

    return 0;
}
