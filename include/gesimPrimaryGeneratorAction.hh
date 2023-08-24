#ifndef gesimPrimaryGeneratorAction_h
#define gesimPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
class G4ParticleGun;
//class G4GeneralParticleSource;
class G4Event;
class gesimDetectorConstruction; //added with the previous version

class gesimPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  gesimPrimaryGeneratorAction(gesimDetectorConstruction*);
  virtual ~gesimPrimaryGeneratorAction();

public:
  virtual void GeneratePrimaries(G4Event* anEvent);

private:
  //G4GeneralParticleSource* fParticleGun;
  G4ParticleGun* fParticleGun;
  gesimDetectorConstruction*    gesimDetector;
  
};

#endif



