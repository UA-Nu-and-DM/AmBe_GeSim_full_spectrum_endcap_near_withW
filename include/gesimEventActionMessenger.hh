#ifndef gesimEventActionMessenger_h
#define gesimEventActionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class gesimEventAction;
class G4UIcmdWithAString;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class gesimEventActionMessenger: public G4UImessenger
{
  public:
    gesimEventActionMessenger(gesimEventAction*);
    virtual ~gesimEventActionMessenger();
    
    virtual void SetNewValue(G4UIcommand*, G4String);
    
  private:
    gesimEventAction*   fEventAction;   
    G4UIcmdWithAString* fDrawCmd;
    G4UIcmdWithAString* fPrintEventsCmd;
};

#endif






