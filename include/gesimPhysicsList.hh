#ifndef gesimPhysicsList_h
#define gesimPhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"
#include <vector>

class G4VPhysicsConstructor;
class gesimPhysicsListMessenger;
class G4ProductionCuts;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class gesimPhysicsList: public G4VModularPhysicsList
{
public:
  gesimPhysicsList();
  virtual ~gesimPhysicsList();

  virtual void ConstructParticle();

  virtual void SetCuts();
  void SetCutForGamma(G4double);
  void SetCutForElectron(G4double);
  void SetCutForPositron(G4double);

  void SelectPhysicsList(const G4String& name);
  virtual void ConstructProcess();

  void SetTargetCut(G4double val);
  void SetDetectorCut(G4double val);

private:

  void AddExtraBuilders(G4bool flagHP);

  // hide assignment operator
  gesimPhysicsList & operator=(const gesimPhysicsList &right);
  gesimPhysicsList(const gesimPhysicsList&);

  G4double fCutForGamma;
  G4double fCutForElectron;
  G4double fCutForPositron;

  G4VPhysicsConstructor*  fEmPhysicsList;
  G4VPhysicsConstructor*  fRaddecayList;
  G4VPhysicsConstructor*  fParticleList;
  G4VPhysicsConstructor*  fHadPhysicsList;

  std::vector<G4VPhysicsConstructor*>  fHadronPhys;
  G4int fNhadcomp;  

  gesimPhysicsListMessenger* fPMessenger;
  G4ProductionCuts* fDetectorCuts;
  G4ProductionCuts* fTargetCuts;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

