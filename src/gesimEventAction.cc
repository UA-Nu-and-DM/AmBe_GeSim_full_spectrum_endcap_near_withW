#include "G4ios.hh"
#include "gesimEventActionMessenger.hh"
#include "gesimEventAction.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UImanager.hh"
#include "G4UnitsTable.hh"
#include "G4PhysicalConstants.hh"
#include "gesimAnalysisManager.hh"

extern G4bool drawEvent;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

gesimEventAction::gesimEventAction()
  : fDrawFlag("all")
{
  fEventMessenger = new gesimEventActionMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

gesimEventAction::~gesimEventAction()
{
  delete fEventMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void gesimEventAction::BeginOfEventAction(const G4Event* )
{
  gesimAnalysisManager::GetInstance()->BeginOfEvent();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void gesimEventAction::EndOfEventAction(const G4Event* evt)
{

  G4int nVertex = evt->GetNumberOfPrimaryVertex();
  for ( G4int j = 0; j < nVertex; j++) { 
    G4int nPart =  evt->GetPrimaryVertex(j)->GetNumberOfParticle(); 
    for ( G4int i = 0; i < nPart; i++) {
      G4ThreeVector position=evt->GetPrimaryVertex(j)->GetPosition();
      G4ThreeVector direction=evt->GetPrimaryVertex(j)->GetPrimary(i)
                                 ->GetMomentum();
      G4double P=direction.mag();
      G4double E=evt->GetPrimaryVertex(j)->GetPrimary(i)->GetMass();
      G4double E0=evt->GetPrimaryVertex(j)->GetPrimary(i)->GetG4code()
                                 ->GetPDGMass();
      E=std::sqrt(P*P+E0*E0);   
      E -= E0; 
      G4double pid = G4double(evt->GetPrimaryVertex(j)->GetPrimary(i)
                                 ->GetPDGcode());
      //  
      direction=direction*(1./direction.mag());    
      direction = -direction;  // reverse the direction
      G4double x,y,z,w;
      G4double Phi=direction.phi();
      if (Phi <0) Phi=Phi+twopi;
      G4double Theta=direction.theta();
      x=position.x();
      y=position.y();
      z=position.z();
      w = evt->GetPrimaryVertex(j)->GetPrimary(i)->GetWeight();
      if(fPrintEventsFlag == "yes") G4cout << "Primary Vertex: " << pid << " " << E << " " << x << " " << y << " " << z << " " << Theta << " " << Phi << " " << w << G4endl;
      //G4cout << " Direction: " << sin(Theta)*cos(Phi) << " " << sin(Theta)*sin(Phi) << " " << cos(Theta) << G4endl;
      //G4cout << sin(Theta)*cos(Phi) << " " << sin(Theta)*sin(Phi) << " " << cos(Theta) << G4endl;
//      exGPSAnalysisManager::getInstance()->Fill(pname,E,x,y,z,Theta,Phi,w);
     //exGPSAnalysisManager::GetInstance()->Fill(pid,E,x,y,z,Theta,Phi,w);
    }   
  }   

  //analysis
  gesimAnalysisManager::GetInstance()->EndOfEvent(evt);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
