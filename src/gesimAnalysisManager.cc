#include "gesimAnalysisManager.hh"
#include "G4UnitsTable.hh"
#include "gesimHisto.hh"
#include "G4ProcessTable.hh"
#include "G4RadioactiveDecay.hh"
#include "G4TwoVector.hh"
#include "G4SystemOfUnits.hh"
#include "G4Event.hh"
#include "G4ios.hh"
#include "Randomize.hh"
#include "CLHEP/Random/RandGauss.h"

#include <iostream>
#include <fstream>
using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

gesimAnalysisManager* gesimAnalysisManager::fManager = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

gesimAnalysisManager* gesimAnalysisManager::GetInstance()
{
  if(!fManager) {
    fManager = new gesimAnalysisManager();
  }
  return fManager;
}
void gesimAnalysisManager::Dispose()
{
  delete fManager;
  fManager = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

gesimAnalysisManager::gesimAnalysisManager()
: fVerbose(0), fNEvt1(-1), fNEvt2(-2),
  fHistEMax (15.0*MeV), fHistEMin (0.),fHistNBin(100),
  fTargetThresE(10*keV), fDetectorThresE(10*keV),fPulseWidth(1.0*microsecond)
{
  fEdepo.clear();
  fHisto   = new gesimHisto();
  BookHisto();
  randGauss = new CLHEP::RandGauss(new CLHEP::RanecuEngine);
  fAMessenger = new gesimAnalysisMessenger(this);
  fDetId = "2";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

gesimAnalysisManager::~gesimAnalysisManager()
{
#ifdef G4ANALYSIS_USE 
#define HISTDELETE
#endif
#ifdef G4ANALYSIS_USE_ROOT
G4cout << "G4ANALYSIS_USE_ROOT"<< G4endl;
#define HISTDELETE
#endif
#ifdef HISTDELETE
//  delete fHisto;
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void gesimAnalysisManager::BookHisto()
{
  fHistEMax = 10.0*MeV;
  fHistEMin = .0*MeV;
  fHistNBin = 10000;

  fHisto->Add1D("H10", "Energy deposit (MeV) in the detector",fHistNBin,fHistEMin,fHistEMax,MeV);
  fHisto->Add1D("Nevts", "Number of primary events simulated",1,0,1);
  /*
  fHisto->Add1D("H11", "Energy deposit (MeV) in the detector (obsolete)",fHistNBin,fHistEMin,fHistEMax,MeV);
  fHisto->Add1D("H12", "Total energy spectrum (MeV) of the target and detector",fHistNBin, fHistEMin,fHistEMax,MeV);
  fHisto->Add1D("H13", "Coincidence spectrum (MeV) between the target and detector",fHistNBin, fHistEMin,fHistEMax,MeV);
  fHisto->Add1D("H14", "Anti-coincidence spectrum (MeV) in the target",fHistNBin, fHistEMin,fHistEMax,MeV);
  fHisto->Add1D("H15", "Anti-coincidence spectrum (MeV) in the detector",fHistNBin, fHistEMin,fHistEMax,MeV);
  fHisto->Add1D("H16", "Decay emission spectrum (MeV)",fHistNBin,fHistEMin,fHistEMax,MeV); 
  // */
  // in aida these histos are indiced from 0-6
  //fHisto->AddTuple( "T1", "Emitted Particles", "double PID, Energy, Time, Weight" );
  //fHisto->AddTuple( "T2", "RadioIsotopes","double PID, Time, Weight" );
  fHisto->AddTuple( "T1", "Hits","double Energy, TrueEnergy, GenEnergy, GenX, GenY, GenZ" );
  //fHisto->AddTuple( "T2", "Muon","double PID, atomA, atomZ, Energy, X, Y, Z" );
  //fHisto->AddTuple( "T3", "Edep","double PID, atomA, atomZ, Energy, X, Y, Z" );
  //fHisto->AddTuple( "RDecayProducts", "All Products of RDecay", "double PID, Z, A, Energy, Time, Weight" );
  // */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void gesimAnalysisManager::BeginOfRun()
{
  std::ofstream ofs;
  ofs.open("Particle_position_VRML.txt", ios::trunc);

  std::ofstream iso;
  iso.open("Particle_position_Momentum_Angular_Distribution.txt", ios::trunc);

  const G4long * table_entry = CLHEP::HepRandom::getTheSeeds();
  G4cout << "Random Seed: " << *table_entry << ", " << *(table_entry+1) << G4endl;
  G4cout << "Random Seed Index: " << CLHEP::HepRandom::getTheSeed() << G4endl;

  fHisto->Book();
  G4cout << "gesimAnalysisManager: Histograms are booked and the run has been started" << G4endl;
  G4ProcessTable *pTable = G4ProcessTable::GetProcessTable();
  G4RadioactiveDecay * rDecay = (G4RadioactiveDecay *) pTable->FindProcess("RadioactiveDecay", "GenericIon");
  if (rDecay != NULL) {
    if (!rDecay->IsAnalogueMonteCarlo()) {
      std::vector<G4RadioactivityTable*> theTables = rDecay->GetTheRadioactivityTables();
      for (size_t i = 0 ; i < theTables.size(); i++) {
          theTables[i]->GetTheMap()->clear();
          G4cout << " Clear the radioactivity map: 0, new size = " << theTables[i]->GetTheMap()->size() << G4endl;
      }
    }  
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void gesimAnalysisManager::EndOfRun(G4int nevent)
{
  fHisto->Save();
  G4cout << "gesimAnalysisManager: Histograms have been saved!" << G4endl;

  // Output the induced radioactivities
  //   in VR mode only
  //
  G4ProcessTable *pTable = G4ProcessTable::GetProcessTable();
  G4RadioactiveDecay * rDecay = (G4RadioactiveDecay *) pTable->FindProcess("RadioactiveDecay", "GenericIon");
  if (rDecay != NULL) {
    if (!rDecay->IsAnalogueMonteCarlo()) {
      G4String fileName = fHisto->GetFileName() + ".activity" ;
      std::ofstream outfile (fileName, std::ios::out );
      std::vector<G4RadioactivityTable*> theTables = rDecay->GetTheRadioactivityTables();
      for (size_t i = 0 ; i < theTables.size(); i++) {
            G4double rate, error;
            outfile << "Radioactivities in decay window no. " << i << G4endl;
            outfile << "Z \tA \tE \tActivity (decays/window) \tError (decays/window) " << G4endl;
            map<G4ThreeVector,G4TwoVector> *aMap = theTables[i]->GetTheMap();
            map<G4ThreeVector,G4TwoVector>::iterator iter;
            for(iter=aMap->begin(); iter != aMap->end(); iter++) {
              rate = iter->second.x()/nevent;
              error = std::sqrt(iter->second.y())/nevent;
              if ( rate < 0.) rate = 0.; // statically it can be < 0. but it's unphysical
              outfile << iter->first.x() <<"\t"<< iter->first.y() <<"\t" << iter->first.z() << "\t" << rate <<"\t" << error << G4endl;
            }
            outfile << G4endl;
      }
      outfile.close();
    }
  }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void gesimAnalysisManager::BeginOfEvent()
{
  fEdepo.clear();
  fHisto->FillHisto(1,0.5,1); // nsimed
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void gesimAnalysisManager::EndOfEvent(const G4Event* evt){

  G4double GenE = evt->GetPrimaryVertex()->GetPrimary()->GetTotalEnergy();
  G4double GenX = 0;
  G4double GenY = 0;
  G4double GenZ = 0;

  G4double MomX = 0;
  G4double MomY = 0;
  G4double MomZ = 0;

//  if(fSaveGen == "yes"){
    GenX = evt->GetPrimaryVertex()->GetPosition().x();
    GenY = evt->GetPrimaryVertex()->GetPosition().y();
    GenZ = evt->GetPrimaryVertex()->GetPosition().z();

    // VRML Visualization of particle position
    std::ofstream ofs;
    ofs.open("Particle_position_VRML.txt", ios::app);
    ofs << "#---------- SPHERE" <<  G4endl;
    ofs << "Transform {" << G4endl;
    ofs << "      translation " << GenX << " " << GenY << " " << GenZ << "," << G4endl;
    ofs <<  " children [" <<  G4endl;
    ofs <<  "   Shape {" <<  G4endl;
    ofs << "      appearance Appearance {" <<  G4endl;
    ofs << "          material Material {" <<  G4endl;
    ofs << "          transparency 0.0 diffuseColor 1 0 0" <<  G4endl;
    ofs << "          emissiveColor 0 0 1" <<  G4endl;
    ofs << "          }" <<  G4endl;
    ofs << "      }" <<  G4endl;
    ofs << "        geometry Sphere {" <<  G4endl;
    ofs << "            radius 0.05" <<  G4endl;
    ofs << "        }" << G4endl;   
    ofs << "    }" << G4endl;   
    ofs << "  ]" << G4endl;
    ofs << "}" << G4endl;
    ofs << G4endl;
    ofs.close();

    MomX = evt->GetPrimaryVertex()->GetPrimary()->GetMomentumDirection().x();
    MomY = evt->GetPrimaryVertex()->GetPrimary()->GetMomentumDirection().y();
    MomZ = evt->GetPrimaryVertex()->GetPrimary()->GetMomentumDirection().z();

    //G4cout << "Mom: " << MomX << " " << MomY << " " << MomZ << G4endl;

    std::ofstream iso;
    iso.open( "Particle_position_Momentum_Angular_Distribution.txt", ios::app);
    
    G4double Phi  = atan(MomY/MomX); // Phi, Theta -- direction of random axis
    G4double cosTheta= MomZ/sqrt(MomX*MomX + MomY*MomY + MomZ*MomZ);
    
    iso <<  GenX << "     " << GenY << "     " << GenZ << "     "  << Phi << "     " << cosTheta << G4endl;
    iso.close();
  //}

  if (fEdepo.size()) {
    std::sort(fEdepo.begin(),fEdepo.end());
    G4double DetE = fEdepo[0].GetEnergy();
    G4double DetW = fEdepo[0].GetEnergy()*fEdepo[0].GetWeight();
    G4double DetT = fEdepo[0].GetTime();

    for (size_t i = 1; i < fEdepo.size(); i++) {
      G4double edep = fEdepo[i].GetEnergy();
      G4double time = fEdepo[i].GetTime();
      G4double weight = fEdepo[i].GetWeight();
      // Edeps in the same time window are added up as one hit
      if (std::fabs(time - DetT) <= fPulseWidth) {
        //G4cout << "time - DetT = " << time - DetT << " <= " << fPulseWidth << " = fPulseWidth" << G4endl; //DEBUG
        // Inside time window
        DetE += edep;
        DetW += edep*weight;
      } else {
        // Outside time window
        // Fill histogram
        if (DetE){
          DetW /= DetE;
          //fHisto->FillHisto(0,DetE,DetW); // Detector histogram
          fHisto->FillTuple(0,0,SimulateEnergyResolution(DetE)/MeV);
          fHisto->FillTuple(0,1,DetE/MeV);
          fHisto->FillTuple(0,2,GenE/MeV);
          fHisto->FillTuple(0,3,GenX/mm);
          fHisto->FillTuple(0,4,GenY/mm);
          fHisto->FillTuple(0,5,GenZ/mm);
          fHisto->AddRow(0);
        }
        // Reset time window start time
        DetE = edep;
        DetT = time;
        DetW = edep*weight;
      }
    }
    // Fill the last hit
    if (DetE){
      DetW /= DetE;
      //fHisto->FillHisto(0,DetE,DetW); // Detector histogram
      fHisto->FillTuple(0,0,SimulateEnergyResolution(DetE)/MeV);
      fHisto->FillTuple(0,1,DetE/MeV);
      fHisto->FillTuple(0,2,GenE/MeV);
      fHisto->FillTuple(0,3,GenX/mm);
      fHisto->FillTuple(0,4,GenY/mm);
      fHisto->FillTuple(0,5,GenZ/mm);
      fHisto->AddRow(0);
    }
    // now add zero energy to separate events
    AddEnergy(0.,0.,0.,0.);

  }else{  // no hits
    fHisto->FillTuple(0,0,0.);
    fHisto->FillTuple(0,1,0.);
    fHisto->FillTuple(0,2,GenE/MeV);
    fHisto->FillTuple(0,3,GenX/mm);
    fHisto->FillTuple(0,4,GenY/mm);
    fHisto->FillTuple(0,5,GenZ/mm);
    fHisto->AddRow(0);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void gesimAnalysisManager::SelectDetector(G4String s){
  fDetId = s;
  G4String detName;
  if(fDetId == "2"){
    detName = "GeII";
  }else if (fDetId == "3"){
    detName = "GeIII";
  }else if (fDetId == "4"){
    detName = "GeIV";
    G4cout << "WARNING: Energy Smearing determined by data taken at UA.\n";
  }
  G4cout << "AnalysisManager: Energy Smearing for Detector: "<< detName << "\n";
}

void gesimAnalysisManager::SaveGen(G4String s){
  fSaveGen = s;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double gesimAnalysisManager::SimulateEnergyResolution(G4double energy){
  G4double eta = randGauss->shoot();
  G4double sigma = 0;
  //G4double sigma = 1.443866*keV + 0.001437 * energy;  //wrong, in channels.
  if(fDetId == "2"){
    sigma = 0.454428*keV + 5.148e-4 * energy; // based on th228-pos1 actual data
  }else if (fDetId == "3"){
    sigma = 0.559765*keV + 2.95347e-4 * energy; // based on th228, ba133 at pos1, actual data
  }else if (fDetId == "4"){
    sigma = 0.646230*keV + 3.36643e-4 * energy; // based on th228, ba133, co60, c057 on det face, actual data taken at UA
  }
  G4double result = energy + eta * sigma;
  //G4cout << "True energy = " << energy << ", Fudged energy = " << result << ", sigma = " << sigma << G4endl;
  return result;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void gesimAnalysisManager::AddEnergy(G4double edep, G4double weight, G4double time, G4int pid)
{
  if(1 < fVerbose) {
    G4cout << "gesimAnalysisManager::AddEnergy: e(keV)= " << edep/keV 
           << " weight = " << weight << " time (s) = " <<  time/second
           << G4endl;
  }
  /* //
  fHisto->FillTuple(1,0,edep/MeV);
  fHisto->FillTuple(1,1,time/second);
  fHisto->FillTuple(1,2,weight);
  fHisto->FillTuple(1,3,pid);
  fHisto->AddRow(1);
  // */ 

  G4double atomZ = -1; 
  G4double atomA = -1; 
  if(pid > 1e9){
    atomZ = (pid/10000)%1000;
    atomA = (pid/10)%1000;
  }
  //AddParticle(2,pid,atomA,atomZ,edep,0,0,0);
  gesimEnergyDeposition A(edep,time,weight);
  fEdepo.push_back(A);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
/* 
void gesimAnalysisManager::AddParticle(G4double pid, G4double energy,
                                       G4double weight, G4double time )
{
  if(1 < fVerbose) {
    G4cout << "gesimAnalysisManager::AddParticle: " << pid
           << G4endl;
  }
  fHisto->FillTuple(0,0, pid);
  fHisto->FillTuple(0,1,energy/MeV);
  fHisto->FillTuple(0,2,time/second);
  fHisto->FillTuple(0,3,weight);
  fHisto->AddRow(0);
  // now fill th emission spectrum
  if (energy>0.0) fHisto->FillHisto(6,energy/MeV,weight);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void gesimAnalysisManager::AddIsotope(G4double pid,G4double weight,
                                      G4double time )
{
  if(1 < fVerbose) {
    G4cout << "gesimAnalysisManager::AddIsotope: " << pid
           << G4endl;
  }
  fHisto->FillTuple(1,0,pid);
  fHisto->FillTuple(1,1,time/second);
  fHisto->FillTuple(1,2,weight);
  fHisto->AddRow(1);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
// */ 
void gesimAnalysisManager::AddDecayProduct(G4double pid,G4int Z, G4int A,
                                           G4double energy, G4double time,
                                           G4double weight)
{
  fHisto->FillTuple(1,0,pid);
  fHisto->FillTuple(1,1,double(Z));
  fHisto->FillTuple(1,2,double(A));
  fHisto->FillTuple(1,3,energy);
  fHisto->FillTuple(1,4,time/s);
  fHisto->FillTuple(1,5,weight);
  fHisto->AddRow(1);
} // */

void gesimAnalysisManager::AddParticle(G4int treenum, G4int pid, G4int A, G4int Z, G4double energy, G4double x, G4double y, G4double z)
{
  if(1 < fVerbose) {
    G4cout << "gesimAnalysisManager::AddParticle: " << pid
           << G4endl;
  }
  fHisto->FillTuple(treenum,0,pid);
  fHisto->FillTuple(treenum,1,A);
  fHisto->FillTuple(treenum,2,Z);
  fHisto->FillTuple(treenum,3,energy/MeV);
  fHisto->FillTuple(treenum,4,x/m);
  fHisto->FillTuple(treenum,5,y/m);
  fHisto->FillTuple(treenum,6,z/m);
  fHisto->AddRow(treenum);
}
