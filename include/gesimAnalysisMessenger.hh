#ifndef gesimAnalysisMessenger_h
#define gesimAnalysisMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class gesimAnalysisManager;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class gesimAnalysisMessenger: public G4UImessenger
{
public:
  
  gesimAnalysisMessenger(gesimAnalysisManager* );
  virtual ~gesimAnalysisMessenger();
    
  virtual void SetNewValue(G4UIcommand*, G4String);
    
private:
  
  gesimAnalysisManager* fPAnalysisManager;

  G4UIdirectory*             fAnaDir;  
  G4UIcmdWithAString*        fSelDetCmd;
  G4UIcmdWithAString*        fSaveGenCmd;
    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

