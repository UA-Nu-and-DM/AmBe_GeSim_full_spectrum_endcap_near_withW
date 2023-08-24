#include "gesimAnalysisMessenger.hh"

#include "gesimAnalysisManager.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

gesimAnalysisMessenger::gesimAnalysisMessenger(gesimAnalysisManager* pAM) :fPAnalysisManager(pAM) {   
  fAnaDir = new G4UIdirectory("/gesim/analysis/");
  fAnaDir->SetGuidance("analysis control.");

  fSelDetCmd = new G4UIcmdWithAString("/gesim/analysis/selectDetector",this);
  fSelDetCmd->SetGuidance("Select detector for energy folding.");
  fSelDetCmd->SetParameterName("SelDet",false);
  fSelDetCmd->AvailableForStates(G4State_PreInit);

  fSaveGenCmd = new G4UIcmdWithAString("/gesim/analysis/saveGen",this);
  fSaveGenCmd->SetGuidance("Save primary vertex info."); 
  fSaveGenCmd->SetParameterName("SaveGen",false);
  fSaveGenCmd->AvailableForStates(G4State_PreInit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

gesimAnalysisMessenger::~gesimAnalysisMessenger() {
  delete fAnaDir;
  delete fSelDetCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void gesimAnalysisMessenger::SetNewValue(G4UIcommand* command, G4String newValue){
  if(command == fSelDetCmd){
    fPAnalysisManager->SelectDetector(newValue);
  }else if(command == fSaveGenCmd){
    fPAnalysisManager->SaveGen(newValue);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
