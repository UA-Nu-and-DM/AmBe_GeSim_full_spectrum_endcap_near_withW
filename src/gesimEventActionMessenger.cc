#include "gesimEventActionMessenger.hh"

#include "gesimEventAction.hh"
#include "G4UIcmdWithAString.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

gesimEventActionMessenger::gesimEventActionMessenger(
                                                               gesimEventAction* EvAct)
:fEventAction(EvAct)
{ 
  fDrawCmd = new G4UIcmdWithAString("/event/draw",this);
  fDrawCmd->SetGuidance("Draw the tracks in the event");
  fDrawCmd->SetGuidance("  Choice : none, charged, all (default)");
  fDrawCmd->SetParameterName("choice",true);
  fDrawCmd->SetDefaultValue("all");
  fDrawCmd->SetCandidates("none charged all");
  fDrawCmd->AvailableForStates(G4State_Idle);

  fPrintEventsCmd = new G4UIcmdWithAString("/event/printEvents",this);
  fPrintEventsCmd->SetGuidance("Print generated events");
  fPrintEventsCmd->SetGuidance("  Choice : yes, no (default)");
  fPrintEventsCmd->SetParameterName("choice",true);
  fPrintEventsCmd->SetDefaultValue("no");
  fPrintEventsCmd->SetCandidates("no yes");
  fPrintEventsCmd->AvailableForStates(G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

gesimEventActionMessenger::~gesimEventActionMessenger()
{
  delete fDrawCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void gesimEventActionMessenger::SetNewValue(G4UIcommand * command,
                                                                      G4String newValue)
{ 
  if(command == fDrawCmd)
    {fEventAction->SetDrawFlag(newValue);}
  else if(command == fPrintEventsCmd)
    {fEventAction->SetPrintEvents(newValue);}
    
   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....





