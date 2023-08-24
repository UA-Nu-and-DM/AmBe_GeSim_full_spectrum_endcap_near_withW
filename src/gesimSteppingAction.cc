#include "G4ios.hh"
#include "G4IonTable.hh"
#include "gesimSteppingAction.hh"
#include "gesimAnalysisManager.hh"
#include "G4Track.hh"
#include "globals.hh"
#include "G4SteppingManager.hh"
#include "G4ParticleDefinition.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

gesimSteppingAction::gesimSteppingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

gesimSteppingAction::~gesimSteppingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void gesimSteppingAction::UserSteppingAction(const G4Step* fStep) 
{
  G4Track* fTrack = fStep->GetTrack();
  G4int StepNo = fTrack->GetCurrentStepNumber();
  G4int maxStep = 10000;
  
  //if(StepNo%100 == 0) G4cout << "Step " << StepNo << G4endl;
  if(StepNo >= maxStep){ 
    fTrack->SetTrackStatus(fStopAndKill);
    G4cout << "Max step number " << maxStep << " reached. Track killed.\n";
  }


  G4ParticleDefinition* thePartDef = fTrack->GetDefinition();
  if(G4IonTable::IsIon(thePartDef)){
      G4int Z=thePartDef->GetAtomicNumber();
      G4int A=thePartDef->GetAtomicMass();
      if((Z==93 && A==237) || (Z==2 && A==4)){
         fTrack->SetTrackStatus(fStopAndKill);
    //G4cout << "Np237 track killed.\n";
      }
  }
  


#ifdef G4ANALYSIS_USE 
#define COLLECT
#endif
#ifdef G4ANALYSIS_USE_ROOT
#ifndef COLLECT
#define COLLECT
#endif
#endif

#ifdef COLLECT
  G4double time = fStep->GetPreStepPoint()->GetGlobalTime();
  G4double weight =  fStep->GetPreStepPoint()->GetWeight();
  G4double edep = fStep->GetTotalEnergyDeposit();
  G4int pid = fTrack->GetDefinition()->GetPDGEncoding();

  gesimAnalysisManager* analysis = gesimAnalysisManager::GetInstance();

  /*/ Ra226
  if(StepNo == 1) { //beginning of step
    G4int pdgcode = G4int(fTrack->GetDefinition()->GetPDGEncoding());
    G4double energy = fStep->GetPreStepPoint()->GetKineticEnergy();
    G4int tid = fTrack->GetTrackID();
    //G4ParticleDefinition* thePartDef = fTrack->GetDefinition();
    G4int pid = fTrack->GetParentID();

    //if(tid == 2){
    //  G4cout << "Parent " << pid << " " << pdgcode << " " << energy << G4endl; 
    //}

    if(pid == 2 && pdgcode == 22){ // && 0.1860 < energy && energy < 0.1864){
      G4cout << "Gamma " << pid << " " << pdgcode << " " << energy*1e3 << " keV" << G4endl; 
    }
    //if(pdgcode == 22 && pid == 1){ // Gamma produced by primary particle
    //}
  } */


  /*if(StepNo == 1) { //beginning of step
    G4double pid=G4double(fTrack->GetDefinition()->GetPDGEncoding());
    G4double energy = fStep->GetPreStepPoint()->GetKineticEnergy();
    G4ParticleDefinition* thePartDef = fTrack->GetDefinition();
    G4String partType= fTrack->GetDefinition()->GetParticleType();
    //if(( partType == "nucleus") &&  !(thePartDef->GetPDGStable()) && fStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "Target" ) {
    //      analysis->AddIsotope(pid, weight, time);
    //}
    if(fTrack->GetTrackID() != 1 ){
      //Radioactive decay products
      if (fTrack->GetCreatorProcess()->GetProcessName() == "RadioactiveDecay") {
        //all products
        G4int Z=thePartDef->GetAtomicNumber();
        G4int A=thePartDef->GetAtomicMass();
        analysis->AddDecayProduct(pid, Z,  A, energy, time, weight);
        // emitted particles except nuclei
        //if(partType!= "nucleus") {
        //  analysis->AddParticle(pid, energy, weight, time);
        //}
      }
    }
  } // */

  /*/ Muon or its spallation products enters the world (from the ceiling)
  if(fStep->GetPostStepPoint()->GetStepStatus() == fGeomBoundary && fStep->GetPostStepPoint()->GetPhysicalVolume()->GetName() == "World"){
    G4ThreeVector position = fStep->GetPostStepPoint()->GetPosition();
    G4double energy = fStep->GetPostStepPoint()->GetKineticEnergy();
    G4double x = position.x();
    G4double y = position.y();
    G4double z = position.z();
    G4ParticleDefinition* thePartDef = fTrack->GetDefinition();
    G4int Z=thePartDef->GetAtomicNumber();
    G4int A=thePartDef->GetAtomicMass();

    // Record particle info
    //G4cout << pid << " " << A << " " << Z << " " << energy << " " << x << " " << y << " " << z << G4endl;
    analysis->AddParticle(1,pid,A,Z,energy,x,y,z);

    // Do not propagate the particle any further
    fTrack->SetTrackStatus(fStopAndKill);
  } // */

  // energy deposition
  //if (fTrack->GetTrackID() != 1  && edep) {
  if (edep && fStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "ActiveDetector"){
  /*
   if (fStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "Detector"){
     edep = -edep;
   } // */
    G4double pid=G4double(fTrack->GetDefinition()->GetPDGEncoding());
    //G4ThreeVector pos = fStep->GetPreStepPoint()->GetPosition();
    //if(pid < 1e9) analysis->AddEnergy(edep, weight, time, pid);
    analysis->AddEnergy(edep, weight, time, pid);
  }
#endif 
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....


