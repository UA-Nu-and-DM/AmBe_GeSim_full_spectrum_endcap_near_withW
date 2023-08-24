#ifndef gesimPhysListHadron_h
#define gesimPhysListHadron_h 1

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"

#include "G4HadronElasticProcess.hh"

#include "G4ProtonInelasticProcess.hh"
#include "G4NeutronInelasticProcess.hh"
#include "G4DeuteronInelasticProcess.hh"
#include "G4TritonInelasticProcess.hh"
#include "G4AlphaInelasticProcess.hh"
#include "G4IonInelasticProcess.hh"
#include "G4HadronFissionProcess.hh"
#include "G4HadronCaptureProcess.hh"


class gesimPhysListHadron : public G4VPhysicsConstructor 
{
  public:
    gesimPhysListHadron(const G4String& name = "hadron");
    virtual ~gesimPhysListHadron();

  public:
  // Construct particle and physics
    virtual void ConstructParticle() {};
  //
    virtual void ConstructProcess();

  private:

  G4HadronElasticProcess  fTheElasticProcess;
  G4ProtonInelasticProcess fTheProtonInelastic;
  G4NeutronInelasticProcess  fTheNeutronInelastic;
  G4HadronElasticProcess* fTheNeutronElasticProcess;
  G4HadronFissionProcess* fTheFissionProcess;
  G4HadronCaptureProcess* fTheCaptureProcess;
  G4DeuteronInelasticProcess* fTheDeuteronInelasticProcess;
  G4TritonInelasticProcess* fTheTritonInelasticProcess;
  G4AlphaInelasticProcess* fTheAlphaInelasticProcess;
  G4IonInelasticProcess* fTheIonInelasticProcess;
};

#endif



