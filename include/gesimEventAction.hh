#ifndef gesimEventAction_h
#define gesimEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class gesimEventActionMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class gesimEventAction : public G4UserEventAction
{
  public:
    gesimEventAction();
    virtual ~gesimEventAction();

  public:
    virtual void BeginOfEventAction(const G4Event* anEvent);
    virtual void EndOfEventAction(const G4Event* anEvent);
    
    void SetDrawFlag(G4String val)  {fDrawFlag = val;};
    void SetPrintEvents(G4String val)  {fPrintEventsFlag = val;};
    
  private:
    G4String fDrawFlag;                         // control the drawing of event
    G4String fPrintEventsFlag;                         // control printing of generated events 
    gesimEventActionMessenger*  fEventMessenger;
};

#endif

    




