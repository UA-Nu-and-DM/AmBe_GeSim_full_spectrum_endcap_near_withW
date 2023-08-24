#ifndef gesimSteppingAction_h
#define gesimSteppingAction_h 1

#include "G4UserSteppingAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class gesimSteppingAction : public G4UserSteppingAction
{
  public:
    gesimSteppingAction();
    virtual ~gesimSteppingAction();

    virtual void UserSteppingAction(const G4Step*);
};

#endif
