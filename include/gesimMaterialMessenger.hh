#ifndef gesimMaterialMessenger_h
#define gesimMaterialMessenger_h 1
////////////////////////////////////////////////////////////////////////////////
//
#include "globals.hh"
#include "G4UImessenger.hh"

#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"
#include "G4UIparameter.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"

class gesimMaterial;
////////////////////////////////////////////////////////////////////////////////
//
class gesimMaterialMessenger: public G4UImessenger
{
public:
  gesimMaterialMessenger(gesimMaterial* );
  virtual ~gesimMaterialMessenger();

  virtual void SetNewValue (G4UIcommand*, G4String);

private:

  gesimMaterial                *fMaterialsManager;

  G4UIdirectory             *fMaterialDir;
  G4UIcmdWithoutParameter   *fListCmd;
  G4UIcmdWithAnInteger      *fDeleteIntCmd;
  G4UIcmdWithAString        *fDeleteNameCmd;
  G4UIcommand               *fAddCmd;
};
////////////////////////////////////////////////////////////////////////////////
#endif
