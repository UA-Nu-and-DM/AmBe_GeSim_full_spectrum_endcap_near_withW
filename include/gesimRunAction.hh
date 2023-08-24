#ifndef gesimRunAction_h
#define gesimRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class G4Run;

class gesimRunAction : public G4UserRunAction
{
  public:
    gesimRunAction();
    virtual ~gesimRunAction();

  public:
    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);
  private:

};

#endif

