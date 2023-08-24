#ifndef gesimPhysListParticles_h
#define gesimPhysListParticles_h 1

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class gesimPhysListParticles : public G4VPhysicsConstructor
{
public: 
  gesimPhysListParticles(const G4String& name = "particles");
  virtual ~gesimPhysListParticles();

public: 
  // This method will be invoked in the Construct() method. 
  // each particle type will be instantiated
  virtual void ConstructParticle();
 
  // This method is dummy.
  virtual void ConstructProcess() {};

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif








