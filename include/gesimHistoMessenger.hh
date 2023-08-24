#ifndef gesimHistoMessenger_h
#define gesimHistoMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class gesimHisto;
class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithAString;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class gesimHistoMessenger: public G4UImessenger
{
  public:

   gesimHistoMessenger(gesimHisto* );
   virtual ~gesimHistoMessenger();

   virtual void SetNewValue(G4UIcommand* ,G4String );

  private:

   gesimHisto*                  fHisto;
   
   G4UIdirectory*          fHistoDir;   
   G4UIcmdWithAString*     fActoryCmd;
   G4UIcmdWithAString*     fIleCmd;
   G4UIcommand*            fHistoCmd;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
