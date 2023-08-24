#include "G4Event.hh"
//#include "G4GeneralParticleSource.hh"
#include "G4ParticleGun.hh"
#include "Randomize.hh"
#include "CLHEP/Random/Random.h"
#include "gesimPrimaryGeneratorAction.hh"

#include "G4HEPEvtInterface.hh"
#include "gesimDetectorConstruction.hh"
#include "G4Tubs.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4HEPEvtInterface.hh"
#include "G4SystemOfUnits.hh"
#include "G4Neutron.hh"
#include "G4Gamma.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
G4VPrimaryGenerator* HEPEvt;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

gesimPrimaryGeneratorAction::gesimPrimaryGeneratorAction(gesimDetectorConstruction* gesimDC):gesimDetector(gesimDC),fParticleGun(0)
{

  HEPEvt = new G4HEPEvtInterface("generator.data");

}

gesimPrimaryGeneratorAction::~gesimPrimaryGeneratorAction()
{

  delete HEPEvt;

}

void gesimPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

   float unit=1*CLHEP::mm;

   float ssShieldTopVoidHeight = 0.89*unit;

   float ssShieldBottomVoidHeight = 0.25267*unit; // ssShieldInnerDia/2*tg(22.5deg) if the bore was made with 135deg point angle drill bit
   float ssShieldCentralVoidHeight = 5.11*unit-ssShieldBottomVoidHeight;
   float ssShieldHeight = 6.50*unit;






   float wShieldHeight = 4.19*unit;
   float wShieldBoreHeight = 4*unit;


   float wWireHeight = 1.8*unit;
   float wWireTop = 0.3*unit;



   float srcCoreHeight = wShieldBoreHeight-(wWireHeight-wWireTop);
   float srcCoreOffset = 0.5*wShieldHeight - (wWireHeight-wWireTop)-0.5*srcCoreHeight;


   float wCapsCapHeight = 21*unit;

   float wCapsBaseOuterDia = 18*unit;
   float wCapsBaseHeight = 16*unit;
   float wCapsBaseBottomHeight = 9*unit;
   float wCapsBaseBoreHeight = 7.62*unit;
   float wCapsBaseBoreOffset = wCapsBaseOuterDia/2 - 5*unit;

   static const G4double inch = 2.54*cm;
   float fEndCapLength = 159.0*mm;
   float fHeadLength = (81.5-13.0)*mm; ; 
   float fDetectorEnclosureLength = fEndCapLength + fHeadLength;
   float fNeckOffset = 30.0*mm; // distance from neck center to "back" of head
   float fNeckLocation = 8.125*inch; //distance from inner cavity center to neck center.

   double endCapFace = fDetectorEnclosureLength - fNeckOffset - fNeckLocation;



auto srcGenOffsetZ = wCapsBaseHeight-0.5*wCapsBaseBottomHeight-wCapsBaseBoreHeight+0.5*ssShieldHeight+0.5*ssShieldHeight-ssShieldTopVoidHeight-0.5*ssShieldCentralVoidHeight+srcCoreOffset;


   auto srcGenOffsetY = wCapsBaseBoreOffset;//when source is in the outer W capsule
   

   auto theta = 270.*deg;//angle of the outer W capsule rotation wrt the outer W capsule z axis. Set to 0 if no outer capsule



    CLHEP::HepRandom::setTheEngine(new CLHEP::MTwistEngine);
    CLHEP::HepRandom::setTheSeed((unsigned)clock());


    auto sourcePV  = gesimDetector->GetSrcCorePV();
    auto sourceLV = sourcePV->GetLogicalVolume();
    auto sourceS = sourceLV->GetSolid();
 //G4cout << "endCapFace PrimaryGeneratorAction :" << endCapFace << G4endl;
   auto srcCtrInCvtyCord =  G4ThreeVector(endCapFace+0.5*wCapsBaseOuterDia,0*CLHEP::mm,-0.5*wCapsCapHeight);;
   

    auto source = (G4Tubs*)sourceS;
    auto rSource = source->GetOuterRadius();
    auto hSource=source->GetZHalfLength();
    auto rSourceRnd = rSource*G4UniformRand();
    auto alpha = CLHEP::twopi*G4UniformRand();   
   
    auto xRnd = rSourceRnd*std::cos(alpha)+srcGenOffsetY*std::sin(theta);
    auto yRnd = rSourceRnd*std::sin(alpha)+srcGenOffsetY*std::cos(theta);
    auto zRnd = hSource*(2*G4UniformRand()-1)+srcGenOffsetZ;



    G4ThreeVector position(srcCtrInCvtyCord.getX()+xRnd, srcCtrInCvtyCord.getY()+yRnd, srcCtrInCvtyCord.getZ()+zRnd);


    HEPEvt->SetParticlePosition(position);


    HEPEvt->GeneratePrimaryVertex(anEvent);

}



