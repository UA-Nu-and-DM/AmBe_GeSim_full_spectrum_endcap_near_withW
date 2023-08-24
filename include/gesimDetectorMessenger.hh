#ifndef gesimDetectorMessenger_h
#define gesimDetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class gesimDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class gesimDetectorMessenger: public G4UImessenger
{
  public:
    gesimDetectorMessenger(gesimDetectorConstruction*);
    virtual         ~gesimDetectorMessenger();
    
    virtual void SetNewValue(G4UIcommand*, G4String);
    
  private:
    gesimDetectorConstruction* fMyDetector;
    
    G4UIdirectory*             fGesimDir;
    G4UIdirectory*             fDetDir;
    G4UIdirectory*             fSourceDir;
    G4UIcmdWithAString*        fSelectDetCmd;
    G4UIcmdWithAString*        fSetSourcePositionCmd;
    G4UIcmdWithADoubleAndUnit* fDeadLayerTweakCmd;
    G4UIcmdWithADoubleAndUnit* fMisalignmentCmd;
    /*
    G4UIcmdWithAString*        fTargMatCmd;
    G4UIcmdWithAString*        fDetectMatCmd;
    G4UIcmdWithADoubleAndUnit* fTargRadiusCmd;
    G4UIcmdWithADoubleAndUnit* fDetectThicknessCmd;
    G4UIcmdWithADoubleAndUnit* fTargLengthCmd;
    G4UIcmdWithADoubleAndUnit* fDetectLengthCmd; // */
    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

