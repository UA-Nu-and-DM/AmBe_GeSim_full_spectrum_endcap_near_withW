#include "gesimDetectorMessenger.hh"

#include "gesimDetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

gesimDetectorMessenger::gesimDetectorMessenger(gesimDetectorConstruction* myDet)
:fMyDetector(myDet)
{ 
  fGesimDir = new G4UIdirectory("/gesim/");
  fGesimDir->SetGuidance("UI commands specific to this example.");
  
  fDetDir = new G4UIdirectory("/gesim/detector/");
  fDetDir->SetGuidance("detector control.");

  fSourceDir = new G4UIdirectory("/gesim/source/");
  fSourceDir->SetGuidance("detector control.");

  fSelectDetCmd = new G4UIcmdWithAString("/gesim/detector/selectDetector",this);
  fSelectDetCmd->SetGuidance("Select Detector (\"2\" for GeII, \"3\" for GeIII).");
  fSelectDetCmd->SetParameterName("choice",false);
  fSelectDetCmd->AvailableForStates(G4State_PreInit);
  
  fSetSourcePositionCmd = new G4UIcmdWithAString("/gesim/source/setSourcePosition",this);
  fSetSourcePositionCmd->SetGuidance("Set source position (\"1\" for the usual spot for calibration, \"2\" for ceiling, \"3\" for center of door).");
  fSetSourcePositionCmd->SetParameterName("choice",false);
  fSetSourcePositionCmd->AvailableForStates(G4State_PreInit);
  
  fDeadLayerTweakCmd = new G4UIcmdWithADoubleAndUnit("/gesim/detector/setDeadLayerTweak", this);
  fDeadLayerTweakCmd->SetGuidance("Set dead layer thickness adjustment.");
  fDeadLayerTweakCmd->SetUnitCategory("Length");
  fDeadLayerTweakCmd->SetParameterName("choice",false);
  fDeadLayerTweakCmd->AvailableForStates(G4State_PreInit);
  
  fMisalignmentCmd = new G4UIcmdWithADoubleAndUnit("/gesim/detector/setMisalignment", this);
  fMisalignmentCmd->SetGuidance("Set detector misalignment to east.");
  fMisalignmentCmd->SetUnitCategory("Length");
  fMisalignmentCmd->SetParameterName("choice",false);
  fMisalignmentCmd->AvailableForStates(G4State_PreInit);
  
  /*fTargMatCmd = new G4UIcmdWithAString("/gesim/det/setTargetMate",this);
  fTargMatCmd->SetGuidance("Select Material of the Target.");
  fTargMatCmd->SetParameterName("choice",false);
  fTargMatCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  fTargRadiusCmd = new G4UIcmdWithADoubleAndUnit("/gesim/det/setTargetRadius", this);
  fTargRadiusCmd->SetGuidance("Set the Target Radius.");
  fTargRadiusCmd->SetUnitCategory("Length");
  fTargRadiusCmd->SetParameterName("choice",false);
  fTargRadiusCmd->AvailableForStates(G4State_PreInit);
  
  fTargLengthCmd = new G4UIcmdWithADoubleAndUnit("/gesim/det/setTargetLength", this);
  fTargLengthCmd->SetGuidance("Set the Target Length.");
  fTargLengthCmd->SetUnitCategory("Length");
  fTargLengthCmd->SetParameterName("choice",false);
  fTargLengthCmd->AvailableForStates(G4State_PreInit);

  fDetectMatCmd = new G4UIcmdWithAString("/gesim/det/setDetectorMate",this);
  fDetectMatCmd->SetGuidance("Select Material of the Detector.");
  fDetectMatCmd->SetParameterName("choice",false);
  fDetectMatCmd->AvailableForStates(G4State_PreInit,G4State_Idle);  

  fDetectThicknessCmd = new G4UIcmdWithADoubleAndUnit("/gesim/det/setDetectorThickness",this);
  fDetectThicknessCmd->SetGuidance("Set the Detector Thickness.");
  fDetectThicknessCmd->SetUnitCategory("Length");
  fDetectThicknessCmd->SetParameterName("choice",false);
  fDetectThicknessCmd->AvailableForStates(G4State_PreInit);

  fDetectLengthCmd = new G4UIcmdWithADoubleAndUnit("/gesim/det/setDetectorLength",this);
  fDetectLengthCmd->SetGuidance("Set the Detector Length.");
  fDetectLengthCmd->SetUnitCategory("Length");
  fDetectLengthCmd->SetParameterName("choice",false);
  fDetectLengthCmd->AvailableForStates(G4State_PreInit); // */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

gesimDetectorMessenger::~gesimDetectorMessenger()
{
  /*delete fTargMatCmd;
  delete fDetectMatCmd;
  delete fTargRadiusCmd;
  delete fDetectThicknessCmd;
  delete fTargLengthCmd;
  delete fDetectLengthCmd; // */
  delete fSelectDetCmd;
  delete fDetDir;
  delete fSourceDir;
  delete fGesimDir;
  delete fDeadLayerTweakCmd;
  delete fMisalignmentCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void gesimDetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ 
    if(command == fSelectDetCmd){
      fMyDetector->SelectDetector(newValue);
    }else if(command == fSetSourcePositionCmd){
      fMyDetector->SetSourcePosition(newValue);
    }else if(command == fDeadLayerTweakCmd){
      fMyDetector->SetDeadLayerTweak(fDeadLayerTweakCmd->GetNewDoubleValue(newValue));
    }else if(command == fMisalignmentCmd){
      fMyDetector->SetMisalignment(fMisalignmentCmd->GetNewDoubleValue(newValue));
    }
//  if( command == fTargMatCmd )fMyDetector->SetTargetMaterial(newValue);
//  else if ( command == fTargLengthCmd ) 
//    fMyDetector->SetTargetLength(fTargLengthCmd->GetNewDoubleValue(newValue));
//  else if ( command == fTargRadiusCmd ) 
//    fMyDetector->SetTargetRadius(fTargLengthCmd->GetNewDoubleValue(newValue));
//  else if( command == fDetectMatCmd )
//    fMyDetector->SetDetectorMaterial(newValue);
//  else if (command == fDetectLengthCmd ) 
//    fMyDetector->SetDetectorLength(
//                     fDetectLengthCmd->GetNewDoubleValue(newValue));
//  else if (command == fDetectThicknessCmd ) 
//    fMyDetector->SetDetectorThickness(
//                              fDetectThicknessCmd->GetNewDoubleValue(newValue));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
