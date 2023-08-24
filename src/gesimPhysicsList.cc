#include "gesimPhysicsList.hh"
#include "gesimPhysicsListMessenger.hh"

#include "gesimPhysListParticles.hh"
#include "G4EmStandardPhysics.hh"
#include "gesimPhysListEmLowEnergy.hh"
//#include "gesimPhysListHadron.hh"
#include "G4RegionStore.hh"
#include "G4Region.hh"
#include "G4ProductionCuts.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

#include "G4UnitsTable.hh"
#include "G4LossTableManager.hh"

#include "QGSP_BERT.hh"
#include "QGSP_BIC.hh"
#include "QGSP_BERT_HP.hh"
#include "QGSP_BIC_HP.hh"
#include "G4HadronPhysicsQGSP_BERT_HP.hh"

#include "G4EmExtraPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4IonBinaryCascadePhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4NeutronTrackingCut.hh"
#include "G4DecayPhysics.hh"
#include "Shielding.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

gesimPhysicsList::gesimPhysicsList() :
 G4VModularPhysicsList(),
 fCutForGamma(1.*mm), fCutForElectron(1.*mm),
 fCutForPositron(1.*mm), fHadPhysicsList(0),
 fNhadcomp(0),fDetectorCuts(0), fTargetCuts(0)
{
  G4LossTableManager::Instance();
  defaultCutValue =1.*mm;

  fPMessenger = new gesimPhysicsListMessenger(this);

  SetVerboseLevel(1);

  //default physics
  fParticleList = new G4DecayPhysics();

  //default physics
  fRaddecayList = new G4RadioactiveDecayPhysics();

  // EM physics
  //fEmPhysicsList = new G4EmStandardPhysics();
  fEmPhysicsList = new G4EmLivermorePhysics();
  
  //hadron physics list
  fHadPhysicsList = new G4HadronPhysicsQGSP_BERT_HP();

  //RegisterPhysics(new QGSP_BERT_HP);
  


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

gesimPhysicsList::~gesimPhysicsList()
{
  delete fPMessenger;
  delete fRaddecayList;
  delete fEmPhysicsList;
  if (fHadPhysicsList) delete fHadPhysicsList;
  if (fNhadcomp > 0) {
    for(G4int i=0; i<fNhadcomp; i++) {
      delete fHadronPhys[i];
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void gesimPhysicsList::ConstructParticle()
{
  fParticleList->ConstructParticle();
  fHadPhysicsList->ConstructParticle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void gesimPhysicsList::ConstructProcess()
{
  AddTransportation();
  // em
  fEmPhysicsList->ConstructProcess();
  // decays
  fParticleList->ConstructProcess();
  fRaddecayList->ConstructProcess();
  // had
  if (fNhadcomp > 0) {
    for(G4int i=0; i<fNhadcomp; i++) {
      (fHadronPhys[i])->ConstructProcess();
    }
  }
  if (fHadPhysicsList) fHadPhysicsList->ConstructProcess();
  G4cout << "### gesimPhysicsList::ConstructProcess is done" << G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void gesimPhysicsList::SelectPhysicsList(const G4String& name)
{
  if (verboseLevel>1) {
    G4cout << "gesimPhysicsList::SelectPhysicsList: <" << name << ">" << G4endl;
  }
  /*// default  Had physics
  if (name == "Hadron" && !fHadPhysicsList) {
    fHadPhysicsList = new gesimPhysListHadron("hadron");
  } else if (name == "QGSP_BERT") {
    AddExtraBuilders(false);
    fHadPhysicsList = new QGSP_BERT(); //"std-hadron");
  } else if (name == "QGSP_BIC" && !fHadPhysicsList) {
    AddExtraBuilders(false);
    fHadPhysicsList = new QGSP_BIC(); //"std-hadron");
  } else if (name == "QGSP_BERT_HP"  && !fHadPhysicsList) {
    AddExtraBuilders(true);
    fHadPhysicsList = new QGSP_BERT_HP(); //"std-hadron");
  } else if (name == "QGSP_BIC_HP"  && !fHadPhysicsList) {
    AddExtraBuilders(true);
    fHadPhysicsList = new QGSP_BIC_HP(); //"std-hadron");
  } else if (name == "LowEnergy_EM") {
      delete fEmPhysicsList;
      fEmPhysicsList = new gesimPhysListEmLowEnergy("lowe-em");
  } else if (name == "Standard_EM") {
      delete fEmPhysicsList;
      fEmPhysicsList = new G4EmStandardPhysics();
  } else {
      G4cout << "gesimPhysicsList WARNING wrong or unkonwn <" 
             << name << "> Physics " << G4endl;
  }  // */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/*
void gesimPhysicsList::AddExtraBuilders(G4bool flagHP)
{
  fNhadcomp = 5;
  fHadronPhys.push_back( new G4EmExtraPhysics("extra EM"));
  //fHadronPhys.push_back( new G4HadronElasticPhysics("elastic",verboseLevel,
  //                                                 flagHP));
  fHadronPhys.push_back( new G4StoppingPhysics("stopping",verboseLevel));
  fHadronPhys.push_back( new G4IonBinaryCascadePhysics("ionBIC"));
  fHadronPhys.push_back( new G4NeutronTrackingCut("Neutron tracking cut"));
}*/

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void gesimPhysicsList::SetCuts()
{
  SetCutValue(fCutForGamma, "gamma");
  SetCutValue(fCutForElectron, "e-");
  SetCutValue(fCutForPositron, "e+");
  G4cout << "world cuts are set" << G4endl;

//  if( !fTargetCuts ) SetTargetCut(fCutForElectron);
//  G4Region* region = (G4RegionStore::GetInstance())->GetRegion("Target");
//  region->SetProductionCuts(fTargetCuts);
//  G4cout << "Target cuts are set" << G4endl;
//
//  if( !fDetectorCuts ) SetDetectorCut(fCutForElectron);
//  region = (G4RegionStore::GetInstance())->GetRegion("Detector");
//  region->SetProductionCuts(fDetectorCuts);
//  G4cout << "Detector cuts are set" << G4endl;

  if (verboseLevel>0) DumpCutValuesTable();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void gesimPhysicsList::SetCutForGamma(G4double cut)
{
  fCutForGamma = cut;
  SetParticleCuts(fCutForGamma, G4Gamma::Gamma());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void gesimPhysicsList::SetCutForElectron(G4double cut)
{
  fCutForElectron = cut;
  SetParticleCuts(fCutForElectron, G4Electron::Electron());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void gesimPhysicsList::SetCutForPositron(G4double cut)
{
  fCutForPositron = cut;
  SetParticleCuts(fCutForPositron, G4Positron::Positron());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void gesimPhysicsList::SetTargetCut(G4double cut)
{
  if( !fTargetCuts ) fTargetCuts = new G4ProductionCuts();

  fTargetCuts->SetProductionCut(cut, idxG4GammaCut);
  fTargetCuts->SetProductionCut(cut, idxG4ElectronCut);
  fTargetCuts->SetProductionCut(cut, idxG4PositronCut);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void gesimPhysicsList::SetDetectorCut(G4double cut)
{
  if( !fDetectorCuts ) fDetectorCuts = new G4ProductionCuts();

  fDetectorCuts->SetProductionCut(cut, idxG4GammaCut);
  fDetectorCuts->SetProductionCut(cut, idxG4ElectronCut);
  fDetectorCuts->SetProductionCut(cut, idxG4PositronCut);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
