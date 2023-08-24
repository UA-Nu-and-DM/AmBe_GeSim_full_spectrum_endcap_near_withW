//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file radioactivedecay/rdecay02/rdecay02.cc
/// \brief Main program of the radioactivedecay/rdecay02 example
//

//#include <iostream>
//#include <stdio.h>

#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "gesimDetectorConstruction.hh"
#include "gesimPhysicsList.hh"
#include "gesimEventAction.hh"
#include "gesimRunAction.hh"
#include "gesimSteppingAction.hh"
#include "gesimPrimaryGeneratorAction.hh"
#include "gesimAnalysisManager.hh"
#include "Randomize.hh"
#include "G4StepLimiterPhysics.hh"
#include "ShieldingLEND.hh"
#include "FTFP_BERT.hh"
#include "QGSP_BIC.hh"
#include "QGSP_BIC_HP.hh"
#include "QGSP_BERT.hh"
#include "QGSP_BERT_HP.hh"
#include "QGSP_INCLXX.hh"
#include "QGSP_INCLXX_HP.hh"
//#include "LBE.hh"
//#include "ArgonLanscePhysicsList.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
//#include "Shielding.hh"
//Geant4 has removed these variables instead choosing to take a very small hit
//for leaving them on even if you do not want them.
//#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
//#endif

//#ifdef G4UI_USE
#include "G4UIExecutive.hh"
//#endif

/*// For executing system commands
std::string exec(char* cmd) {
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while(!feof(pipe)) {
    	if(fgets(buffer, 128, pipe) != NULL)
    		result += buffer;
    }
    pclose(pipe);
    return result;
} // */

// Main Program
int main(int argc,char** argv)
{
  // random engine
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);

  // Print original seeds
  //const G4long * table_entry = CLHEP::HepRandom::getTheSeeds();
  //G4cout << "Main: Random Seed: " << *table_entry << ", " << *(table_entry+1) << G4endl;

  // Set new seeds based on current time (nanosecond part)
  //const G4long seed = atoi(exec("date +%N").c_str());
  //G4long seeds[2] = {seed, table_entry[1]};
  //G4long seeds[2] = {12345,67890}; // Setting seeds manually
  //CLHEP::HepRandom::setTheSeeds(seeds);
  //G4cout << "Main: New Random Seed: " << *seeds << ", " << *(seeds+1) << G4endl;
  //G4cout << "Random Seed: " << CLHEP::HepRandom::getTheSeed() << G4endl;
  

  // Construct the default run manager
  G4RunManager* runManager = new G4RunManager;

  // Creation of the analysis manager
  gesimAnalysisManager::GetInstance();

  // set mandatory initialization classes

  gesimDetectorConstruction* Detector = new gesimDetectorConstruction;
  runManager->SetUserInitialization(Detector);
  runManager->SetUserInitialization(new gesimPhysicsList);
  //G4VModularPhysicsList* physlist;
  //physlist = new ShieldingLEND();
  //physlist->RegisterPhysics(new G4DecayPhysics(1));
  //physlist->RegisterPhysics(new G4RadioactiveDecayPhysics(1));
  //runManager->SetUserInitialization(physlist);
  //runManager->SetUserInitialization(new Shielding(1, "LEND"));
  //auto physList = new QGSP_BERT_HP;
  //physList->RegisterPhysics(new G4StepLimiterPhysics());
  //runManager->SetUserInitialization(physList);
  // set mandatory user action class
  
  //runManager->SetUserAction(new gesimPrimaryGeneratorAction);
  runManager->SetUserAction(new gesimPrimaryGeneratorAction(Detector));
  runManager->SetUserAction(new gesimRunAction);
  runManager->SetUserAction(new gesimEventAction);
  runManager->SetUserAction(new gesimSteppingAction);

  //#ifdef G4VIS_USE
  // visualization manager
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
  //#endif

  // Initialize G4 kernel
  // do this at run time so the geometry/physics can be changed
  //  runManager->Initialize();

  // get the pointer to the User Interface manager 
  G4UImanager* UImanager = G4UImanager::GetUIpointer();  

  if (argc == 1)   // Define UI session for interactive mode.
    {
      //#ifdef G4UI_USE
      G4UIExecutive* ui = new G4UIExecutive(argc, argv);
      ui->SessionStart();
      delete ui;
      //#endif
    }
  else           // Batch mode
    { 
      G4String command = "/control/execute ";
      G4String fileName = argv[1];
      UImanager->ApplyCommand(command+fileName);
    }
  
  // job termination
  //#ifdef G4VIS_USE
  delete visManager;
  //#endif
  gesimAnalysisManager::Dispose();
  delete runManager;

  return 0;
}








